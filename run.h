#include <stdio.h>
#include "cmd.h"


word w_read(unsigned short int adr);

extern Command cmd[];

extern word reg[8];
#define pc reg[7]

void run() {
    pc = 01000;
    int i = 0;
    while (1) {
        word w = w_read(pc);
        printf("%06o %06o ", pc, w);
        i = 0;
        while ((w & cmd[i].mask ) != cmd[i].opcode) {
            i++;
            }
        printf("%s\n", cmd[i].name);
        cmd[i].func(NULL);
        pc+=2;
    }
}
