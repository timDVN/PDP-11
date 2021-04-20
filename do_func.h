#include "run.h"
#include "set_flags.h"

typedef word Adress;


extern Arg ss;
extern Arg dd;


void w_write(Adress adr, word w);
void b_write(Adress adr, byte b);



void do_add() {

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
    set_Z(ss.val);
}

void do_nothing() {
    trace_func("\n");
    print_r();
    exit(1);
}

void do_br() {
    if (((X >> 7) & 1) != 0)
        X = X - 0400;
    pc = pc + 2 * X;
}

void do_beq() {
    if (flag_Z == 1)
        do_br();
}

void do_bpl() {
    if (flag_Z == 1)
        do_br();
}

void do_halt() {
    trace_func("\n");
    print_r();
    trace_func(" NZC = %06o\n", 4*flag_N + 2*flag_Z + flag_C);
    exit(0);
}
void do_ash() {
}
void do_ashc() {
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
    trace_func(" NZC = %02o\n", 4*flag_N + 2*flag_Z + flag_C);

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
    trace_func("NZC = %02o\n", 4*flag_N + 2*flag_Z + flag_C);
}

void do_com() {

}

void do_jsr() {
    reg[R] = pc;
    pc = dd.val;
}

void do_rol() {
}

void do_ror() {
}

void do_div(){
}

void do_sob(){
    if ( --reg[R] != 0)
    {
        pc = pc - 2 * N;
    }
}

void do_clr(){
    if (B == 0)
        w_write(dd.adr, 0);
    else
        b_write(dd.adr, 0);
    flag_C = 0;
    flag_N = 0;
    flag_Z = 1;
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