#include <stdio.h>
#include <stdlib.h>

typedef unsigned short int word;
word w_read(unsigned short int adr);

typedef struct arg{
    word val; // значние аргумента
    word adr; // адрес аргумента
}Arg;

extern unsigned char mem[64*1024];
extern word reg[8];

Arg get_mr(word w){
    Arg res;
    int r = w & 7;
    int mode = (w >> 3) & 7;
    switch (mode) {
        case 0:
            res.val = reg[r];
            res.adr = r;
#ifdef PPP
            printf("R%o ", r);
#endif
            break;
        case 1:
            res.adr = reg[r];
            res.val = w_read(res.adr); // b_read
#ifdef PPP
            printf("R%o ", r);
#endif
            break;
        case 2:
            res.adr = reg[r];
            res.val = w_read(res.adr);// b_read
            reg[r] += 2; // +1
#ifdef PPP
if (r == 7)
    {
    printf("#%o", res.val);
    }
    else
        printf("R%o ", r);
#endif
            break;
        case 3:
            res.adr = w_read(reg[r]);
            res.val = w_read(res.adr);
            reg[r] += 2;
#ifdef PPP
            if (r == 7)
    {
    printf("@#%o", res.adr);
    }
    else
        printf("R%o ", r);
#endif

            break;
        case 4:
            reg[r] -= 2;// -1
            res.adr = reg[r];
            res.val = w_read(res.adr);

#ifdef PPP
            printf("R%o ", r);
#endif
            break;
        case 5:
            reg[r] -= 2;
            res.adr = reg[r];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
#ifdef PPP
            printf("R%o ", r);
#endif
            break;
        default:
            printf("Mode %o is in development\n", mode);
            exit(1);
    }
    return res;
}