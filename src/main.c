#include <proto/exec.h>
#include <proto/dos.h>

#include "debug.h"
#include "pkt.h"

/* main entry for console handler */
int ConMain(void)
{
  struct MsgPort *mp;
  struct DosPacket *dp;
  struct Message *msg;

  /* receive init packet */
  D(("Con wait\n"));
  mp = &((struct Process*)FindTask(NULL))->pr_MsgPort;
  WaitPort(mp);
  dp = (struct DosPacket*)GetMsg(mp)->mn_Node.ln_Name;
  D(("Con got first: @%lx type=%ld\n", dp, dp->dp_Type));

  /* TODO: do my setup */

  /* ack first packet */
  D(("Con ack first\n"));
  pkt_reply(dp, DOSTRUE);

  /* main loop */
  D(("Con main loop\n"));
  for (;;) {
    /* wait for event */
    ULONG mp_mask = 1L << mp->mp_SigBit;
    ULONG sigs = Wait(mp_mask);

    /* a dos packet arrived */
    while(1) {
      msg = GetMsg(mp);
      if(msg == NULL) {
        break;
      }
      dp = (struct DosPacket*)msg->mn_Node.ln_Name;
      dp->dp_Res2 = 0;
      D(("Con got pkt: @%lx type=%ld\n", dp, dp->dp_Type));
      switch (dp->dp_Type) {
        default:
          D(("  unknown action!\n"));
          pkt_reply2(dp, DOSFALSE, ERROR_ACTION_NOT_KNOWN);
          break;
      }
    }
  }

  D(("Con done\n"));
  return 0;
}
