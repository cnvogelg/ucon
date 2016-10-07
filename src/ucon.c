
#include <proto/exec.h>
#include <proto/dos.h>

#include "ucon.h"
#include "debug.h"

struct ucon_fh *ucon_init(void)
{
  /* alloc file handle */
  struct ucon_fh *fh = AllocVec(sizeof(struct ucon_fh), MEMF_PUBLIC | MEMF_CLEAR);
  if (!fh) {
    D(("no fh mem!\n"));
    return NULL;
  }

  return fh;
}

void ucon_exit(struct ucon_fh *fh)
{
  FreeVec(fh);
}

LONG ucon_write(struct ucon_fh *fh, UBYTE *buffer, LONG length)
{
  for(LONG i=0;i<length;i++) {
    KPutCh(buffer[i]);
  }
  return length;
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
