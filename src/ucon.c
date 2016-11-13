
#include <proto/exec.h>
#include <proto/dos.h>

#include <resources/ucon.h>
#define __NOLIBBASE__
#include <proto/ucon.h>
#define UconBase (fh->ucon_res)

#include "ucon.h"
#include "debug.h"
#include "pkt.h"

struct ucon_fh *ucon_init(void)
{
  /* find ucon.resource */
  struct UconResource *res = OpenResource("ucon.resource");
  if(res == NULL) {
    D(("no ucon.resource!\n"));
    return NULL;
  }

  /* alloc signal */
  BYTE read_sig = AllocSignal(-1);
  if(read_sig == -1) {
    D(("no signal!\n"));
    return NULL;
  }

  /* alloc file handle */
  struct ucon_fh *fh = AllocVec(sizeof(struct ucon_fh), MEMF_PUBLIC | MEMF_CLEAR);
  if (!fh) {
    D(("no fh mem!\n"));
    return NULL;
  }

  fh->ucon_res = res;
  fh->read_sig = read_sig;
  fh->read_sig_mask = 1 << read_sig;
  D(("res %lx, read sig %lx\n", fh->ucon_res, fh->read_sig_mask));

  /* open ucon resource */
  struct Task *myTask = FindTask(NULL);
  LONG result = UconInit(fh->read_sig_mask, myTask);
  D(("open result=%lx\n", result));

  return fh;
}

void ucon_exit(struct ucon_fh *fh)
{
  /* close ucon resource */
  UconExit();

  FreeSignal(fh->read_sig);
  FreeVec(fh);
}

void ucon_open(struct ucon_fh *fh, APTR this_fh, UBYTE *name, LONG type)
{
  UconOpen(this_fh, name, type);
}

void ucon_close(struct ucon_fh *fh, APTR this_fh)
{
  UconClose(this_fh);
}

void ucon_write(struct ucon_fh *fh, struct DosPacket *dp, UBYTE *buffer, LONG length)
{
  /* sync write and direct reply */
  LONG result = UconWrite(buffer, length);
  pkt_reply2(dp, result, 0);
}

void ucon_read(struct ucon_fh *fh, struct DosPacket *dp, UBYTE *buffer, LONG length)
{
  /* check if read is pending? */
  if(fh->read_pkt != NULL) {
    D(("read: already pending??\n"));
    pkt_reply2(dp, 0, 0);
    return;
  }

  /* store current packet */
  fh->read_pkt = dp;

  /* call async read. it will be answered with signal */
  UconRead(buffer, length, &fh->read_result);
}

ULONG ucon_sigmask(struct ucon_fh *fh)
{
  return fh->read_sig_mask;
}

void ucon_handle_sig(struct ucon_fh *fh, ULONG sigmask)
{
  D(("got signal %lx\n", sigmask));
  if(fh->read_pkt == NULL) {
    D(("signal: no pending read??\n"));
  } else {
    D(("read finished: len=%lx\n", fh->read_result));
    pkt_reply2(fh->read_pkt, fh->read_result, 0);
    fh->read_pkt = NULL;
  }
}
