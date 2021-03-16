#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;// 8 bit
typedef unsigned short int word;// 16 bit
typedef word Adress;// 16 bit

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];
void test_mem(char * c[]);
void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);
void load_file(const char * file);


void b_write(Adress adr, byte b) {
    mem[adr] = b;
}

byte b_read(Adress adr) {
    return mem[adr];
}

void w_write(Adress adr, word w) {
    mem[adr + 1] = (byte) (w >> 8);
    mem[adr] = (byte) ((w << 8) >> 8);

}

word w_read(Adress adr) {
    word w = ((word) mem[adr + 1]) << 8;
    w = w | mem[adr];
    return w;
}

void load_file(const char * file) {
    Adress adr, n;
    byte k;
    FILE *f;
    f = fopen(file, "rt");
    fscanf(f,"%hx%hx", &adr, &n);
    for (Adress i = 0; i < n; i++) {
        fscanf(f,"%hhx", &k);
        b_write(adr+i , k);
    }
    fclose(f);

}

int main(int argc, char * argv[]) {
    test_mem(argv);
    return 0;
}

void test_mem(char * c[]) {
    byte b0 = 0xa;
    word w0 = 0xcb0a;
    w_write(4, w0);
    b_write(2, b0);
    assert(b0 == b_read(2));
    assert(w0 == w_read(4));
    load_file(c[1]);
    for (int i = 4; i < 8; i++)
        printf("%hhx ", b_read(i));
}
