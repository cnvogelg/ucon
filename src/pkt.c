#include <proto/exec.h>
#include <proto/dos.h>

void pkt_reply(struct DosPacket *dp, LONG res1)
{
    struct MsgPort *mp;
    struct Message *mn;

    mp = dp->dp_Port;
    mn = dp->dp_Link;
    mn->mn_Node.ln_Name = (char*) dp;
    dp->dp_Port = &((struct Process*) FindTask(NULL))->pr_MsgPort;
    dp->dp_Res1 = res1;
    PutMsg(mp, mn);
}

void pkt_reply2(struct DosPacket *dp, LONG res1, LONG res2)
{
    dp->dp_Res2 = res2;
    pkt_reply(dp, res1);
}
