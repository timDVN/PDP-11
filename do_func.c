#include <stdlib.h>
#include <stdio.h>
#include "lib.h"


void do_add() {
    word res = dd.val + ss.val;
    w_write(dd.adr, res);
    set_N(res);
    set_Z(res);
    set_C(ss.val + dd.val);
}
void do_move() {
    if (B == 0 ) {
        w_write(dd.adr, ss.val);
        set_N(ss.val);
    }
    else {
        b_write(dd.adr, ss.val);
        set_N(ss.val << LEN_BYTE);
    }
    set_Z(ss.val);
}
void do_nothing() {
    trace_func("\n");
    print_r();
    exit(1);
}
void do_br() {
    if (X >> (LEX_X - 1))  // if negative
        X = X - 0400;
    pc = pc + 2 * X;
}
void do_beq() {
    if (flag_Z == 1)
        do_br();
}

void do_bpl() {
    if (flag_N == 0)
        do_br();
}

void do_halt() {
    trace_func("\n");
    print_r();
    print_flags();
    exit(0);
}

void do_asl() {
    word res;
    if (B == 0) {
        res = dd.val * 2;
        w_write(dd.adr, res);
        set_N(res);
        set_Z(res);
        set_C(dd.val * 2);
    }
    else{
        res = (byte)dd.val * 2;
        b_write(dd.adr, res);
        set_N((dd.val*2) << LEN_BYTE);
        set_Z(res);
        set_C((((byte) dd.val) * 2) << LEN_BYTE);
    }
}
void do_asr() {
    word res;
    if (B == 0)
    {
        res = (word) dd.val / 2;
        w_write(dd.adr, res);
        set_Z(res);
        set_N(res);
    }
    else
    {
        res = ((byte) dd.val) / 2;
        b_write(dd.adr, res);
        set_N(res << LEN_BYTE);
        set_Z(res);
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
    print_flags();

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
    print_flags();
}

void do_jsr() {
    reg[6] -= 2;
    w_write(reg[6], reg[R]);
    reg[R] = pc;
    pc = dd.adr;
}

void do_rts(){
    pc = reg[R];
    reg[R] = w_read(reg[6]);
    reg[6] +=2;
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
void set_N(word w)
{
    if ((w >> (LEN_WORD - 1)) & 1)
        flag_N = 1;
    else
        flag_N = 0;
}

void set_C(int w)
{
    if ((w >> LEN_WORD) && (w > 0))
        flag_C = 1;
    else
        flag_C = 0;
}

void set_Z(word w)
{
    if (w == 0)
        flag_Z = 1;
    else
        flag_Z = 0;

}

void print_flags(){
    if(flag_N == 1)
        trace_func("N");
    else
        trace_func("-");
    if(flag_Z == 1)
        trace_func("Z");
    else
        trace_func("-");
    trace_func("-");
    if(flag_C == 1)
        trace_func("C");
    else
        trace_func("-");
}
