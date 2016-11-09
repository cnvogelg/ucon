
#include <proto/exec.h>
#include <proto/dos.h>

#include <resources/ucon.h>
#define __NOLIBBASE__
#include <proto/ucon.h>
#define UconBase (fh->ucon_res)

#include "ucon.h"
#include "debug.h"

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
  LONG result = UconOpen(fh->read_sig_mask, myTask);
  D(("open result=%lx\n", result));

  return fh;
}

void ucon_exit(struct ucon_fh *fh)
{
  /* close ucon resource */
  UconClose();

  FreeSignal(fh->read_sig);
  FreeVec(fh);
}

LONG ucon_write(struct ucon_fh *fh, UBYTE *buffer, LONG length)
{
  return UconWrite(buffer, length);
}

static char dir[] = "dir";
static char endshell[] = "endshell";

LONG ucon_read(struct ucon_fh *fh, UBYTE *buffer, LONG length)
{
  if(fh->fake_state == 0) {
    fh->fake_state++;
    CopyMemQuick( dir, buffer, sizeof(dir) );
    return sizeof(dir);
  }
  else if(fh->fake_state == 1) {
    fh->fake_state++;
    CopyMemQuick( endshell, buffer, sizeof(endshell) );
    return sizeof(endshell);
  }
  return 0;
}

ULONG ucon_sigmask(struct ucon_fh *fh)
{
  return fh->read_sig_mask;
}

void ucon_handle_sig(struct ucon_fh *fh, ULONG sigmask)
{
  D(("got signal %lx\n", sigmask));
}
