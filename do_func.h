#include "run.h"
#include "set_flags.h"

typedef word Adress;


extern Arg ss;
extern Arg dd;


void w_write(Adress adr, word w);
void b_write(Adress adr, byte b);



void do_add() {
    printf("add\n");
    w_write(dd.adr, (word)dd.val + ss.val);
    set_N(dd.val + ss.val);
    set_Z(dd.val + ss.val);
    set_C(ss.val + dd.val);
}

void do_move() {
    if (B == 0 )
        w_write(dd.adr, ss.val);
    else {
        b_write(dd.adr, ss.val);
        set_N(ss.val << 8);
    }
    printf("move\n");
    set_Z(ss.val);
}

void do_nothing() {
    printf("nothing\n");
    print_r();
    exit(1);
}

void do_br() {
    if (((X >> 7) & 1) != 0)
        X = X - 0400;
    pc = pc + 2 * X;
    printf("br\n");
}

void do_beq() {
    printf("BEQ ");
    if (flag_Z == 1)
        do_br();
}

void do_bpl() {
    printf("BPL ");
    if (flag_Z == 1)
        do_br();


}

void do_halt() {
    printf("THE END!!!\n");
    print_r();
    printf("\n");
    for (int i = 0; i < 10; i+=2)
        printf("%06o ", w_read(i + 0x200));
    printf(" NZC = %06o\n", 4*flag_N + 2*flag_Z + flag_C);
    exit(0);
}
void do_ash() {
    printf("ash...\n");

}
void do_ashc() {
    printf("ashc...\n");
}
void do_asl() {
    if (B == 0) {
        w_write(dd.adr, (word) dd.val * 2);
        set_N((word)dd.val * 2);
        set_Z((word)dd.val * 2);
        set_C(dd.val * 2);
    }
    else{
        b_write(dd.adr, (byte)dd.val * 2);
        set_N((dd.val*2) << 8);
        set_Z((byte) dd.val*2);
        set_C((((byte) dd.val) * 2) << 8);
        }
    printf("asl\n");

}
void do_asr() {
    if (B == 0)
    {
        w_write(dd.adr, (word) dd.val / 2);
        set_Z((word) dd.val / 2);
        set_N((word) dd.val / 2);
    }
    else
    {
        b_write(dd.adr, ((byte) dd.val) / 2);
        set_N((((byte)dd.val) / 2) << 8);
        set_Z(((byte)dd.val) / 2);
    }
    printf("asr\n");
    flag_C = 0;

}

void do_tst(){
    if (B == 1)
    {
        dd.val = byte_to_word(dd.val);
    }
    set_Z(dd.val);
    set_N(dd.val);
    flag_C = 0;
    printf("tst NZC = %02o\n", 4*flag_N + 2*flag_Z + flag_C);

}

void do_cmp(){
    word res;
    if (B == 1)
    {
        ss.val = (byte) ss.val;
        dd.val = (byte)dd.val;
    }
    res = ss.val - dd.val;
    set_N(res);
    set_Z(res);
    set_C(ss.val - dd.val);
    printf("cmp NZC = %02o\n", 4*flag_N + 2*flag_Z + flag_C);


}

void do_com() {
    printf("com...\n");
}

void do_jsr() {
    reg[R] = pc;
    pc = dd.val;
    printf("jsr\n");
}
void do_rol() {
    printf("rol...\n");
}
void do_ror() {
    printf("ror...\n");
}
void do_div(){
    printf("div...\n");
}
void do_sob(){
    if ( --reg[R] != 0)
    {
        pc = pc - 2 * N;
    }
    printf("sob\n");
}
void do_clr(){
    if (B == 0)
        w_write(dd.adr, 0);
    else
        b_write(dd.adr, 0);
    flag_C = 0;
    flag_N = 0;
    flag_Z = 1;
    printf("clr\n");
}
void do_scc(){
    flag_N = 1;
    flag_C = 1;
    flag_Z = 1;
}
void do_sec(){
    flag_C = 1;
}
void do_sen(){
    flag_N = 1;
}
void do_sez(){
    flag_Z = 1;
}
void do_ccc(){
    flag_N = 0;
    flag_C = 0;
    flag_Z = 0;
}
void do_clc(){
    flag_C = 0;
}
void do_cln(){
    flag_N = 0;
}
void do_clz(){
    flag_Z = 0;
}