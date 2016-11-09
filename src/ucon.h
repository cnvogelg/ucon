#ifndef UCON_H
#define UCON_H

struct ucon_fh
{
  UWORD         use_count;
  UWORD         fake_state;
  struct Task * break_task;

  struct UconResource *ucon_res;
  ULONG                read_sig;
  ULONG                read_sig_mask;
};

extern struct ucon_fh *ucon_init(void);
extern ULONG ucon_sigmask(struct ucon_fh *fh);
extern void ucon_exit(struct ucon_fh *fh);
extern LONG ucon_write(struct ucon_fh *fh, UBYTE *buffer, LONG length);
extern LONG ucon_read(struct ucon_fh *fh, UBYTE *buffer, LONG length);
extern void ucon_handle_sig(struct ucon_fh *fh, ULONG sigmask);

#endif
