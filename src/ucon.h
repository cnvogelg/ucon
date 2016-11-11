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
  /* current read packet */
  struct DosPacket    *read_pkt;
  LONG                 read_result;
};

extern struct ucon_fh *ucon_init(void);
extern void ucon_exit(struct ucon_fh *fh);

extern void ucon_write(struct ucon_fh *fh, struct DosPacket *dp, UBYTE *buffer, LONG length);
extern void ucon_read(struct ucon_fh *fh, struct DosPacket *dp, UBYTE *buffer, LONG length);

extern ULONG ucon_sigmask(struct ucon_fh *fh);
extern void ucon_handle_sig(struct ucon_fh *fh, ULONG sigmask);

#endif
