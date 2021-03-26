#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "run.h" //cmd.h is imported by run.h; do_func.h is imported by cmd.h

#define PPP
#define TESTT
typedef unsigned char byte;// 8 bit
typedef unsigned short int word;// 16 bit
typedef word Adress;// 16 bit

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void test_mem(char *c[]);

void b_write(Adress adr, byte b);

byte b_read(Adress adr);

void w_write(Adress adr, word w);

word w_read(Adress adr);

void load_file(const char *file);




void b_write(Adress adr, byte b) {
    mem[adr] = b;
}

byte b_read(Adress adr) {
    return mem[adr];
}

void w_write(Adress adr, word w) {
    mem[adr + 1] = (byte) (w >> 8);
    mem[adr] = (byte) (w);
}

word w_read(Adress adr) {
    word w = ((word) mem[adr + 1]) << 8;
    w = w | mem[adr];
    return w;
}

void load_file(const char *file) {
    Adress adr, n;
    byte k;
    FILE * f = fopen(file, "r");
    if (f == NULL)
    {
        perror(file);
        exit(errno);
    }
    fscanf(f, "%hx%hx", &adr, &n);
    for (Adress i = 0; i < n; i++) {
        fscanf(f, "%hhx", &k);
        b_write(adr + i, k);
    }
    fclose(stdin);

}

Command cmd[] = {
        {0170000, 010000, "mov", do_mov},
        {0170000, 060000, "add", do_add},
        {0177777, 000000, "halt", do_halt},
        {0000000, 000000, "unknown command", do_nothing}
};

int main(int argc, char *argv[]) {
#ifdef TESTT
    test_mem(argv);
#endif
    return 0;
}

void test_mem(char *c[]) {
    byte b0 = 0x0a;
    byte b1 = 0xcb;
    word w0 = 0xcb0a;
    b_write(7,b1);
    b_write(6,b0);
    w_write(4, w0);
    b_write(2, b0);
    printf("%06x\n", w_read(6));
    assert(b0 == b_read(2));
    assert(w0 == w_read(4));
    assert(w0 == w_read(6));
    assert(b0 == b_read(4));
    assert(b1 == b_read(5));
    load_file(c[1]);
    for (int i = 0; i < 5; i++)
        printf("%06hx ", b_read(i + 0x200));
    printf("\n");
}
