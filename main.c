#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "do_func.h"
#include "TestFiles/test1.h"
#include "TestFiles/test2.h"
#include "TestFiles/test3.h"
#include "TestFiles/test4.h"
#include "TestFiles/sob_b.h"
#include "TestFiles/arr0.h"
#include "TestFiles/arr0_byte.h"
#include "TestFiles/tst_cmp.h"


#define PPP
#define TESTT

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];
word reg[8];
Arg dd, ss;
word N, X, R;
byte flag_N, flag_Z, flag_C;
byte B;

void b_write(Adress adr, byte b);

byte b_read(Adress adr);




void load_file(const char *file);


void b_write(Adress adr, byte b) {
    if (adr <= 7) {

        reg[adr] = byte_to_word(b);
    }
    else
        mem[adr] = b;
}

byte b_read(Adress adr) {
    if (adr <= 7)
        return (byte)reg[adr];
    else
        return mem[adr];
}

void w_write(Adress adr, word w) {
    if (adr <= 7)
    {
        reg[adr] = w;
    } else
    {
        mem[adr + 1] = (byte) (w >> 8);
        mem[adr] = (byte) (w);
    }

}

word w_read(Adress adr) {
    word w = ((word) mem[adr + 1]) << 8;
    w = w | mem[adr];
    return w;
}

void load_file(const char *file) {
    Adress adr, n;
    byte k;
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        perror(file);
        exit(errno);
    }
    while (fscanf(f, "%hx%hx", &adr, &n) != EOF) {
        for (Adress i = 0; i < n; i++) {
            fscanf(f, "%hhx", &k);
            b_write(adr + i, k);
        }
    }
    fclose(stdin);

}


Command cmd[] = {
        {0077700, 0005000, "CLR", 01, (void (*)(void))(char *)do_clr},
        {0070000, 0010000, "move",03,  (void (*)(void)) (char *) do_move},
        {0170000, 0060000, "add",  03 ,(void (*)(void)) (char *)do_add},
        {0177000, 0072000, "ASH",   012 ,(void (*)(void)) (char *)do_ash},
        {0177000, 0073000, "ASHC",  012 ,(void (*)(void))(char *)do_ashc},
        {0077700, 0006300, "ASL", 01,(void (*)(void))(char *) do_asl},
        {0077700, 0006200, "ASR", 01,(void (*)(void))(char *) do_asr},
        {0177400, 0000400, "BR",  020,(void (*)(void))(char *)do_br},
        {0177400, 0100000, "BPL", 020,(void (*)(void))(char *)do_bpl},
        {0177400, 0001400, "BEQ", 020,(void (*)(void))(char *)do_beq},
        {0177777, 0000241, "CLC",   0,(void (*)(void))(char *) do_clc},
        {0177777, 0000250, "CLN",   0,(void (*)(void))(char *) do_cln},
        {0177777, 0000244, "CLZ",   0,(void (*)(void))(char *)do_clz},
        {0077700, 0005100, "COM", 01,(void (*)(void)) (char *) do_com},
        {0177000, 0004000, "JSR",   011,(void (*)(void))(char *)do_jsr},
        {0077700, 0006100, "ROL", 01,(void (*)(void))(char *)do_rol},
        {0077700, 0006000, "ROR", 01 , (void (*)(void))(char *)do_ror},
        {0177777, 0000277, "SCC",  0 ,(void (*)(void))(char *)do_scc},
        {0177777, 0000257, "CCC", 0,(void (*)(void))(char *)do_ccc},
        {0177777, 0000261, "SEC",   0,(void (*)(void))(char *)do_sec},
        {0177777, 0000270, "SEN",  0,(void (*)(void)) (char *)do_sen},
        {0177777, 0000264, "SEZ",  0,(void (*)(void)) (char *)do_sez},
        {0177000, 0077000, "SOB", 014,(void (*)(void))(char *)do_sob},
        {0177000, 0071000, "DIV", 012,(void (*)(void))(char *)do_div},
        {0077700, 0005700, "TST",01, (void (*)(void))(char *)do_tst},
        {0070000, 0020000, "CMP",03, (void (*)(void))(char *)do_cmp},
        {0177777, 000000,  "halt",0,(void (*)(void))(char *)do_halt},
        {0000000, 000000,  "unknown command", 0,(void (*)(void))(char *)do_nothing}
};

int main(int argc, char *argv[]) {
#ifdef TESTT
    load_file(argv[1]);
    tst_cmp();
#endif
    return 0;
}
