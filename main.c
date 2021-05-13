#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include "lib.h"


byte mem[MEMSIZE];
word reg[8];
Arg dd, ss;
word N, X, R;
byte flag_N, flag_Z, flag_C;
byte B;
char *trc;
FILE *trace;
word dif;


void b_write(Adress adr, byte b){
    if (adr <= 7) {

        reg[adr] = byte_to_word(b);
    } else
        mem[adr] = b;
    if (adr == 0177566 || adr == 0177567)
        printf("%c", w_read(0177566));
}

byte b_read(Adress adr) {
    if (adr <= 7)
        return (byte) reg[adr];
    else
        return mem[adr];
}

void w_write(Adress adr, word w) {
    if (adr <= 7) {
        reg[adr] = w;
    } else {
        mem[adr + 1] = (byte) (w >> 8);
        mem[adr] = (byte) (w);
    }

    if (adr == 0177566)
        printf("%c\n", w);

}

word w_read(Adress adr) {
    word w = ((word) mem[adr + 1]) << 8;
    w = w | mem[adr];
    return w;
}

word byte_to_word(byte b){
    word res;
    if (((b >> 7) & 1) == 1)
        res =  b + 0177400;
    else
        res = b;
    return res;
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
    fclose(f);

}


Command cmd[] = {
        {0077700, 0005000, "CLR",             01,  (void (*)(void)) (char *) do_clr},
        {0070000, 0010000, "move",            03,  (void (*)(void)) (char *) do_move},
        {0170000, 0060000, "add",             03,  (void (*)(void)) (char *) do_add},
        {0177000, 0072000, "ASH",             012, (void (*)(void)) (char *) do_ash},
        {0177000, 0073000, "ASHC",            012, (void (*)(void)) (char *) do_ashc},
        {0077700, 0006300, "ASL",             01,  (void (*)(void)) (char *) do_asl},
        {0077700, 0006200, "ASR",             01,  (void (*)(void)) (char *) do_asr},
        {0177400, 0000400, "BR",              020, (void (*)(void)) (char *) do_br},
        {0177400, 0100000, "BPL",             020, (void (*)(void)) (char *) do_bpl},
        {0177400, 0001400, "BEQ",             020, (void (*)(void)) (char *) do_beq},
        {0177777, 0000241, "CLC",             0,   (void (*)(void)) (char *) do_clc},
        {0177777, 0000250, "CLN",             0,   (void (*)(void)) (char *) do_cln},
        {0177777, 0000244, "CLZ",             0,   (void (*)(void)) (char *) do_clz},
        {0077700, 0005100, "COM",             01,  (void (*)(void)) (char *) do_com},
        {0177000, 0004000, "JSR",             011, (void (*)(void)) (char *) do_jsr},
        {0077700, 0006100, "ROL",             01,  (void (*)(void)) (char *) do_rol},
        {0077700, 0006000, "ROR",             01,  (void (*)(void)) (char *) do_ror},
        {0177777, 0000277, "SCC",             0,   (void (*)(void)) (char *) do_scc},
        {0177777, 0000257, "CCC",             0,   (void (*)(void)) (char *) do_ccc},
        {0177777, 0000261, "SEC",             0,   (void (*)(void)) (char *) do_sec},
        {0177777, 0000270, "SEN",             0,   (void (*)(void)) (char *) do_sen},
        {0177777, 0000264, "SEZ",             0,   (void (*)(void)) (char *) do_sez},
        {0177000, 0077000, "SOB",             014, (void (*)(void)) (char *) do_sob},
        {0177000, 0071000, "DIV",             012, (void (*)(void)) (char *) do_div},
        {0077700, 0005700, "TST",             01,  (void (*)(void)) (char *) do_tst},
        {0070000, 0020000, "CMP",             03,  (void (*)(void)) (char *) do_cmp},
        {0177770, 0000200, "RTS",             0,   (void (*)(void)) (char *) do_rts},
        {0177777, 000000,  "halt",            0,   (void (*)(void)) (char *) do_halt},
        {0000000, 000000,  "unknown command", 0,   (void (*)(void)) (char *) do_nothing}
};

void test_mem1() { //тест на моды, w/b_read/write (for file in1.txt)
    load_file("TestFiles/in1.txt");
    byte b0 = 0x0a;
    byte b1 = 0xcb;
    word w0 = 0xcb0a;
    b_write(15, b1);
    b_write(14, b0);
    w_write(12, w0);
    b_write(10, b0);
    assert(b0 == b_read(10));
    assert(w0 == w_read(12));
    assert(w0 == w_read(14));
    assert(b0 == b_read(12));
    assert(b1 == b_read(13));
    reg[0] = 5;
    reg[1] = 01000;
    reg[2] = 01000;
    reg[3] = 01006;
    reg[4] = 01004;
    reg[5] = 01006;
    Arg res;
    res = get_mr(000);
    assert(res.val == 5);
    res = get_mr(011);
    assert(res.val == 020);
    res = get_mr(022);
    assert(res.val == 020);
    assert(reg[2] == 01002);
    res = get_mr(033);
    assert(res.val == 0x22);
    assert(reg[3] == 01010);
    res = get_mr(044);
    assert(res.val == 0x22);
    assert(reg[4] == 01002);
    res = get_mr(055);
    assert(res.val == 020);
    assert(reg[5] == 01004);
    for (int i = 0; i < 6; i++ )
    {
        reg[i] = 0;
    }
    b_write(15, 0);
    b_write(14, 0);
    w_write(12, 0);
    b_write(10, 0);
    for (int i = 01000; i < 01012; i += 2 )
        w_write(i, 0);
    pc = 01000;
}

int main(int argc, char *argv[]) {
    trc = 0;
    int inp = 1;
    if (argv[1][0] == '-' && argv[1][1] == 't' && argv[1][2] == 0 && argv[2] != 0) {
        int len = strlen(argv[2]);
        char *t = ".trace";
        trc = malloc(sizeof(char) * (strlen(t) + len));
        for (int i = 0; i < len; i++) {
            trc[i] = argv[2][i];
        }
        for (int i = 0; i < 7; i++) {
            trc[i + len] = t[i];
        }
        test_mem1();
        trace = fopen(trc, "w");
        inp = 2;
    }
    load_file(argv[inp]);
    w_write(0177564, 0177777);
    run();
    if (trc != 0)
        fclose(trace);
    return 0;
}

void trace_func(const char *format, ... ){
    va_list  ap;
    va_start(ap, format);
    if (trc == 0)
    {
        vprintf(format, ap);
    } else
    {
        vfprintf(trace,format, ap);
    }

    va_end(ap);
}

void run() {
    pc = 01000;
    int i = 0;
    while (1) {
        word w = w_read(pc);
        trace_func("%06o %06o ", pc, w);
        pc += 2;
        i = 0;
        while ((w & cmd[i].mask ) != cmd[i].opcode) {
            i++;
        }
        trace_func("%s ", cmd[i].name);
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
            w = w >> 6;
            if  (((cmd[i].params >> 1) & 01) == 1)
                w = w >> 6;
        }
        if (((cmd[i].params >> 2) & 01) == 1)
        {
            N = (w & 077);
            w = w >> 6;
            trace_func("%o ", pc - 2*N);
        }
        if (((cmd[i].params >> 3)& 01) == 1)
        {
            R = (w & 07);
            trace_func("R%o ", R);
            w = w >> 3;
        }
        if (((cmd[i].params >> 4)& 01) == 1)
        {
            X = (w & 0377);
            char x = (char)x;
            trace_func("%o ", pc+x*2);
        }
        cmd[i].func();
        trace_func("\n");
    }
}

void  print_r(){
    for (int i = 0; i < 8; i++) {
        trace_func("r%o = %o ; ",i, reg[i]);
    }
}

Arg get_mr(word w){
    Arg res;
    int r = w & 7;
    int mode = (w >> 3) & 7;
    switch (mode) {
        case 0:
            res.val = reg[r];
            res.adr = r;
            trace_func("R%o ", r);
            break;
        case 1:
            res.adr = reg[r];
            if (B == 0)
                res.val = w_read(res.adr);
            else
                res.val = b_read(res.adr);
            trace_func("(R%o) ", r);
            break;
        case 2:
            res.adr = reg[r];
            res.val = B ? b_read(res.adr) : w_read(res.adr);
            if (B == 0 || r == 7 || r == 6) {
                reg[r] += 2;
            } else{
                reg[r] += 1;
            }

            if (r == 7)
                trace_func("#%o ", res.val);
            else
                trace_func("(R%o)+ ", r);
            break;
        case 3:
            res.adr = w_read(reg[r]);
            res.val = w_read(res.adr);
            reg[r] += 2;
            if (r == 7)
                trace_func("@#%o ", res.adr);
            else
                trace_func("@(R%o)+ ", r);
            break;
        case 4:
            if (B == 0 || r == 7 || r == 6) {
                reg[r] -= 2;
            } else{
                reg[r] -= 1;
            }
            res.adr = w_read(reg[r]);
            res.val = B ? b_read(res.adr) : w_read(res.adr);
            trace_func("-(R%o) ", r);

            break;
        case 5:
            reg[r] -= 2;
            res.adr = reg[r];
            res.adr = w_read(res.adr);
            res.val = w_read(res.adr);
            trace_func("@-(R%o) ", r);
            break;
        case 6:
            dif = w_read(pc);
            pc += 2;
            res.adr = reg[r] + dif;
            res.val = w_read(res.adr);
            if (r == 7)
                trace_func("%o ", res.adr);
            else
                trace_func("%o(R%o) ", dif, r);
            break;
        default:
            trace_func("Mode %o is in development\n", mode);
            exit(1);
    }
    return res;
}