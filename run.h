#include <stdio.h>
#include "cmd.h"


extern word w_read(unsigned short int adr);
extern Command cmd[];

word reg[8];
#define pc reg[7]

void run() {
    printf("%s", cmd[1].name);
    pc = 01000;
    while (1) {
        word w = w_read(pc);
#ifdef PPP
        printf("%06o %06o", pc, w);
#endif
        pc += 2;
        int i = 0;
        while (cmd[i].mask != 0){
            if (w & cmd[i].mask == cmd[i].opcode) {
                printf("%s\n", cmd[i].name);
                cmd[i].do_func;
            }

        }

    }
}
