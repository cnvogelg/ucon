#ifndef UCON_H
#define UCON_H

struct ucon_fh
{
  UWORD         use_count;
  UWORD         fake_state;
  struct Task * break_task;
};

extern struct ucon_fh *ucon_init(void);
extern void ucon_exit(struct ucon_fh *fh);
extern LONG ucon_write(struct ucon_fh *fh, UBYTE *buffer, LONG length);
extern LONG ucon_read(struct ucon_fh *fh, UBYTE *buffer, LONG length);

#endif
