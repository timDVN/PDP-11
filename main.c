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


void b_write(Adress adr, byte b) {
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
        mem[adr + 1] = (byte) (w >> LEN_BYTE);
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

word byte_to_word(byte b) {
    word res;
    if (b >> (LEN_BYTE - 1))
        res = b + 0177400;
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
        exit(1);
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
        {0077700, 0005000, "CLR", HAS_DD, do_clr},
        {0070000, 0010000, "move", HAS_SS | HAS_DD, do_move},
        {0170000, 0060000, "add", HAS_DD | HAS_SS, do_add},
        {0077700, 0006300, "ASL", HAS_DD, do_asl},
        {0077700, 0006200, "ASR", HAS_DD, do_asr},
        {0177400, 0000400, "BR", HAS_X, do_br},
        {0177400, 0100000, "BPL", HAS_X, do_bpl},
        {0177400, 0001400, "BEQ", HAS_X, do_beq},
        {0177777, 0000241, "CLC", 0, do_clc},
        {0177777, 0000250, "CLN", 0, do_cln},
        {0177777, 0000244, "CLZ", 0, do_clz},
        {0177000, 0004000, "JSR", HAS_DD | HAS_R, do_jsr},
        {0177777, 0000277, "SCC", 0, do_scc},
        {0177777, 0000257, "CCC", 0, do_ccc},
        {0177777, 0000261, "SEC", 0, do_sec},
        {0177777, 0000270, "SEN", 0, do_sen},
        {0177777, 0000264, "SEZ", 0, do_sez},
        {0177000, 0077000, "SOB", HAS_R | HAS_NN, do_sob},
        {0077700, 0005700, "TST", HAS_DD, do_tst},
        {0070000, 0020000, "CMP", HAS_DD | HAS_SS, do_cmp},
        {0177770, 0000200, "RTS", 0, do_rts},
        {0177777, 000000,  "halt", 0, do_halt},
        {0000000, 000000,  "unknown command", 0, do_nothing}
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
    Arg res_new ={.val = 5, .adr = 0} ;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    res = get_mr(011);
    res_new.adr = 01000;
    res_new.val = 020;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    res = get_mr(022);
    res_new.adr = 01000;
    res_new.val = 020;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    assert(reg[2] == 01002);
    res = get_mr(033);
    res_new.adr = 01002;
    res_new.val = 042;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    assert(reg[3] == 01010);
    res = get_mr(044);
    /*for (int i = 0, i < 12; i+= 2)
    {
      fprintf(stderr,"");
    }*/
    //fprintf(stderr, "res.val == %x  | reg[4] = %o  | B == % d\n", res.val, reg[4], B);
    res_new.adr = 01002;
    res_new.val = 042;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    assert(reg[4] == 01002);
    res = get_mr(055);
    res_new.adr = 01000;
    res_new.val = 020;
    assert(0 == memcmp(&res, &res_new, sizeof(res)));
    assert(reg[5] == 01004);
    for (int i = 0; i < 6; i++) {
        reg[i] = 0;
    }
    b_write(15, 0);
    b_write(14, 0);
    w_write(12, 0);
    b_write(10, 0);
    for (int i = 01000; i < 01012; i += 2)
        w_write(i, 0);
    pc = 01000;
    trace_func("\n");
}

void usage(const char *progname) {
    fprintf(stderr, "USAGE: %s [-t] FILE\n ", progname);
    fprintf(stderr, "\t-t - trace on\n ");
}

int main(int argc, char *argv[]) {
    // нет аргументов, совсем нет
    if (argc == 1) {
        usage(argv[0]);
        return 1;
    }

    // есть аргумент -t? разбираем его
    int inp = 1;
    int trace_level = 0;
    if (strcmp(argv[1], "-t") == 0)
    {
        trace_level = 1;
        inp++;
    }
    // очередной аргумент последний? Это входной файл!
    if (inp + 1 != argc) {
        usage(argv[0]);
        return 1;
    }

    // задаем имя файла с трассировкой
    // если есть флаг -t и
    if (trace_level) {
        const char *ext = ".trace";
        size_t len = strlen(argv[inp]) + strlen(ext);
        char *filename = malloc(len + 1);
        strcpy(filename, argv[inp]);
        strcat(filename, ext);
        printf("filename=<%s>\n", filename);
        trace = fopen(filename, "w");

        if (trace == NULL) {
            perror(filename);
            exit(1);
        }
        free(filename);
    } else
        trace = stderr;

    test_mem1();
    load_file(argv[inp]);
    w_write(0177564, 0177777);
    run();
    if (trace != NULL)
        fclose(trace);
    return 0;
}

void trace_func(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    if (trace == NULL) {
        vprintf(format, ap);
    } else {
        vfprintf(trace, format, ap);
    }

    va_end(ap);
}

void run() {
    pc = 01000;
    int i;
    while (1) {
        word w = w_read(pc);
        trace_func("%06o %06o ", pc, w);
        pc += 2;
        i = 0;
        while ((w & cmd[i].mask) != cmd[i].opcode) {
            i++;
        }
        trace_func("%s ", cmd[i].name);
        if (((w >> POSITION_B) == 1) && ((cmd[i].mask >> POSITION_B) == 0))
            B = 1;
        else
            B = 0;
        if (cmd[i].params & HAS_SS) {
            ss = get_mr(w>>LEN_DD);
        }
        if (cmd[i].params & HAS_DD) {
            dd = get_mr(w);
            w = w >> LEN_DD;
            if (cmd[i].params & HAS_SS)
            {
                w = w >> LEN_SS;
            }
        }
        if (cmd[i].params & HAS_NN) {
            N = (w & 077);
            w = w >> LEN_NN;
            trace_func("%o ", pc - 2 * N);
        }
        if (cmd[i].params & HAS_R) {
            R = (w & 07);
            trace_func("R%o ", R);
            w = w >> LEN_R;
        }
        if (cmd[i].params & HAS_X) {
            X = (w & 0377);
            //char x = (char)x;
            trace_func("%o ", pc + X * 2);
        }
        cmd[i].func();
        trace_func("\n");
    }
}

void print_r() {
    for (int i = 0; i < 8; i++) {
        trace_func("r%o = %o ; ", i, reg[i]);
    }
}

Arg get_mr(word w) {
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
            } else {
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
            } else {
                reg[r] -= 1;
            }
            res.adr = reg[r];
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