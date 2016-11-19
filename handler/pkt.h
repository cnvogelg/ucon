#ifndef PKT_H
#define PKT_H

extern void pkt_reply(struct DosPacket *dp, LONG res1);
extern void pkt_reply2(struct DosPacket *dp, LONG res1, LONG res2);

#endif
