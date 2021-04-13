#include "cmd.h"
#include "args.h"

word w_read(unsigned short int adr);


extern Arg ss;
extern Arg dd;
extern word N;
extern word R;
extern word X;
extern Command cmd[];

#define pc reg[7]

void run() {
    pc = 01000;
    int i = 0;
    while (1) {
        word w = w_read(pc);
        printf("%06o %06o ", pc, w);
        pc += 2;
        i = 0;
        while ((w & cmd[i].mask ) != cmd[i].opcode) {
            i++;
            }
#ifdef PPP
        printf("%s\n", cmd[i].name);
#endif
        if ((((w >> 15) & 01) == 1) && ((( cmd[i].mask>> 15) & 01) == 0))
            B = 1;
        else
            B = 0;
        if  (((cmd[i].params >> 1) & 01) == 1)
        {
            ss = get_mr(w>>6);
        }
        if ((cmd[i].params &  01) == 1)
        {
            dd = get_mr(w);
            w = w >> 12;
        }
        if (((cmd[i].params >> 2) & 01) == 1)
        {
            N = (w & 077);
            w = w >> 6;
        }
        if (((cmd[i].params >> 3)& 01) == 1)
        {
            R = (w & 07);
            w = w >> 3;
        }
        if (((cmd[i].params >> 4)& 01) == 1)
        {
            X = (w & 0377);
        }
        cmd[i].func();
    }
}
