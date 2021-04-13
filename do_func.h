#include "run.h"

typedef word Adress;

extern byte flag_N;
extern byte flag_Z;
extern byte flag_C;
extern Arg ss;
extern Arg dd;


void w_write(Adress adr, word w);
void b_write(Adress adr, byte b);


void  print_r(){
    for (int i = 0; i < 8; i++) {
        printf("r%o = %o; ",i, reg[i]);
    }
    printf("\n");
}

void do_add() {
    printf("add\n");
    w_write(dd.adr, (word)dd.val + ss.val);

}

void do_move() {
    if (B == 0 )
        w_write(dd.adr, ss.val);
    else
        b_write(dd.adr, ss.val);
    printf("move\n");
}

void do_nothing() {
    printf("nothing\n");
    print_r();
    exit(1);
}

void do_br() {
    pc = pc + 2 * X;
    printf("br\n");
}

void do_halt() {
    printf("THE END!!!\n");
    print_r();
    printf("\n");
    for (int i = 0; i < 10; i+=2)
        printf("%06o ", w_read(i + 0x200));
    exit(0);
}
void do_ash() {
    printf("ash...\n");
}
void do_ashc() {
    printf("ashc...\n");
}
void do_asl() {
    if (B == 0)
        w_write(dd.adr, (word)dd.val * 2);
    else
        b_write(dd.adr, (byte)dd.val * 2);
    printf("asl\n");
}
void do_asr() {
    if (B == 0)
        w_write(dd.adr, (word)dd.val / 2);
    else
        b_write(dd.adr, (byte)dd.val / 2);
    printf("asr\n");
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
    pc = pc - 2 * N;
    printf("sob...\n");
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
void do_clc(){
    flag_N = 0;
    flag_C = 0;
    flag_Z = 0;
}
//void do_clc(){
//    flag_C = 0;
//}
void do_cln(){
    flag_N = 0;
}
void do_clz(){
    flag_Z = 0;
}