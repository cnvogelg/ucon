#include <proto/exec.h>
#include <proto/dos.h>

#include "debug.h"
#include "pkt.h"
#include "ucon.h"

/* main entry for console handler */
int ConMain(void)
{
  struct MsgPort *mp;
  struct DosPacket *dp;
  struct Message *msg;
  struct ucon_fh *ucon_fh;

  /* receive init packet */
  D(("Con wait\n"));
  mp = &((struct Process*)FindTask(NULL))->pr_MsgPort;
  WaitPort(mp);
  dp = (struct DosPacket*)GetMsg(mp)->mn_Node.ln_Name;
  D(("Con got first: @%lx type=%ld\n", dp, dp->dp_Type));

  /* ucon setup */
  ucon_fh = ucon_init();
  if(ucon_fh == NULL) {
    D(("Con failed init!\n"));
    pkt_reply(dp, DOSFALSE);
    return 0;
  }

  /* ack first packet */
  D(("Con ack first\n"));
  pkt_reply(dp, DOSTRUE);

  /* main loop */
  D(("Con main loop\n"));
  BOOL stay = TRUE;
  ULONG mp_mask = 1L << mp->mp_SigBit;
  ULONG ucon_mask = ucon_sigmask(ucon_fh);
  ULONG sig_mask = mp_mask | ucon_mask;
  while(stay) {
    /* wait for event */
    ULONG sigs = Wait(sig_mask);

    /* ucon sig */
    if(sigs & ucon_mask) {
      ucon_handle_sig(ucon_fh, sigs);
    }

    /* a dos packet arrived */
    if(sigs & mp_mask) {
      while(stay) {
        msg = GetMsg(mp);
        if(msg == NULL) {
          break;
        }
        dp = (struct DosPacket*)msg->mn_Node.ln_Name;
        dp->dp_Res2 = 0;
        switch (dp->dp_Type) {
          case ACTION_FINDINPUT:
          case ACTION_FINDOUTPUT:
          case ACTION_FINDUPDATE:
            {
              struct FileHandle *dos_fh = BADDR(dp->dp_Arg1);
              dos_fh->fh_Port = (APTR)DOSTRUE; /* == fh_Interactive */
              dos_fh->fh_Arg1 = (ULONG)ucon_fh;
              ucon_fh->use_count++;
              ucon_fh->break_task = dp->dp_Port->mp_SigTask;
              D(("Find X: fh=@%lx use_count=%ld\n", dos_fh, ucon_fh->use_count));
              pkt_reply(dp, DOSTRUE);
              break;
            }
          case ACTION_END:
            {
              ucon_fh->use_count--;
              struct FileHandle *dos_fh = BADDR(dp->dp_Arg1);
              D(("End: fh=@%lx use_count=%ld\n", dos_fh, ucon_fh->use_count));
              if(ucon_fh->use_count == 0) {
                stay = FALSE;
              } else {
                pkt_reply(dp, DOSTRUE);
              }
              break;
            }
          case ACTION_READ:
            {
              UBYTE *buffer = (UBYTE*)dp->dp_Arg2;
              LONG length = dp->dp_Arg3;
              D(("READ: buf=@%lx len=%ld\n", buffer, length));
              LONG result = ucon_read(ucon_fh, buffer, length);
              pkt_reply2(dp, result, 0);
              break;
            }
          case ACTION_WRITE:
            {
              UBYTE *buffer = (UBYTE*)dp->dp_Arg2;
              LONG length = dp->dp_Arg3;
              D(("WRITE: buf=@%lx len=%ld\n", buffer, length));
              LONG result = ucon_write(ucon_fh, buffer, length);
              pkt_reply2(dp, result, 0);
              break;
            }
          default:
            D(("Con got pkt: @%lx type=%ld\n", dp, dp->dp_Type));
            pkt_reply2(dp, DOSFALSE, ERROR_ACTION_NOT_KNOWN);
            break;
        }
      }
    }
  }

  D(("Close con\n"));
  ucon_exit(ucon_fh);

  pkt_reply(dp, DOSFALSE);
  D(("Con done\n"));
  return 0;
}
