#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "do_func.h"
#include "run.h" //cmd.h is imported by run.h;

#define PPP
#define TESTT
typedef unsigned char byte;// 8 bit
typedef unsigned short int word;// 16 bit
typedef word Adress;// 16 bit

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];
word reg[8];

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
        {0170000, 0110000, "mov_b", do_mov_b},
        {0170000, 0010000, "mov_w", do_mov_w},
        {0170000, 0060000, "add", do_add},
        {0177000, 0072000, "ASH", do_ash},
        {0177000, 0073000, "ASHC", do_ashc},
        {0177700, 0106300, "ASL_b", do_asl_b},
        {0177700, 0006300, "ASL_w", do_asl_w},
        {0177700, 0106200, "ASR_b", do_asr_b},
        {0177700, 0006200, "ASR_w", do_asr_w},
//        {0177700, 0103000, "BCC", do_bcc},
//        {0177700, 0103400, "BCS", do_bcs},
//        {0177700, 0001400, "BEQ", do_beq},
//        {0177700, 0002000, "BGE", do_bge},
//        {0177700, 0003000, "BGT", do_bgt},
//        {0170000, 0140000, "BIC_b", do_bic_b},
//        {0170000, 0040000, "BIC_w", do_bic_w},
//        {0170000, 0150000, "BIS_b", do_bis_b},
//        {0170000, 0050000, "BIS_w", do_bis_w},
//        {0170000, 0130000, "BIT_b", do_bit_b},
//        {0170000, 0030000, "BIT_w", do_bit_w},
//        {0177700, 0101000, "BHI", do_bhi},
//        {0177700, 0103000, "BHIS", do_bhis},
//        {0177700, 0003400, "BLE", do_ble},
//        {0177700, 0002400, "BLT", do_blt},
//        {0177700, 0103400, "BLO", do_blo},
//        {0177700, 0101400, "BLOS", do_blos},
//        {0177700, 0100400, "BMI", do_bmi},
//        {0177700, 0001000, "BNE", do_bne},
//        {0177700, 0100000, "BPL", do_bpl},
//        {0177700, 0000400, "BR", do_br},
//        {0177700, 0102000, "BVC", do_bvc},
//        {0177700, 0102400, "BVS", do_bvs},
//        {0177700, 0004700, "CALL", do_call},
//        {0177777, 0000257, "CCC", do_ccc},
//        {0177777, 0000241, "CLC", do_clc},
//        {0177777, 0000250, "CLN", do_cln},
//        {0177700, 0005000, "CLR_w", do_clr_w},
//        {0177700, 0105000, "CLR_b", do_clr_b},
//        {0177777, 0000242, "CLV", do_clv},
//        {0177777, 0000244, "CLZ", do_clz},
//        {0170000, 0020000, "CMP_b", do_cmp_b},
//        {0170000, 0120000, "CMP_w", do_cmp_w},
        {0177700, 0005100, "COM_w", do_com_w},
        {0177700, 0105100, "COM_v", do_com_b},
//        {0177700, 0005300, "DEC_w", do_dec_w},
//        {0177700, 0105300, "DEC_b", do_dec_b},
//        {0177700, 0005200, "INC_w", do_inc_w},
//        {0177700, 0105200, "INC_b", do_inc_b},
//        {0177700, 0000100, "JMP", do_jmp},
        {0177000, 0004000, "JSR", do_jsr},
//        {0177700, 0005400, "NEG_w", do_neg_w},
//        {0177700, 0105400, "NEG_b", do_neg_b},
//        {0177777, 0000240, "NOP", do_nop},
//        {0177777, 0000005, "RESET", do_reset},
//        {0177777, 0000207, "RETURN", do_return},
        {0177700, 0006100, "ROL_w", do_rol_w},
        {0177700, 0106100, "ROL_b", do_rol_b},
        {0177700, 0006000, "ROR_w", do_ror_w},
        {0177700, 0106000, "ROR_b", do_ror_b},
//        {0177770, 0000200, "RTS", do_rts},
//        {0177700, 0005600, "SBC_w", do_sbc_w},
//        {0177700, 0105600, "SBC_b", do_sbc_b},
//        {0177777, 0000277, "SCC", do_scc},
//        {0177777, 0000261, "SEC", do_sec},
//        {0177777, 0000270, "SEN", do_sen},
//        {0177777, 0000262, "SEV", do_sev},
//        {0177777, 0000264, "SEZ", do_sez},
//        {0177000, 0077000, "SOB", do_sob},
//        {0170000, 0160000, "SUB", do_sub},
//        {0177700, 0000300, "SWAB", do_swab},
//        {0177700, 0006700, "SXT", do_sxt},
//        {0177700, 0005700, "TST_w", do_tst_w},
//        {0177700, 0105700, "TST_b", do_tst_b},
//        {0177000, 0074000, "XOR", do_xor},
        {0177000, 0071000, "DIV", do_div},
        {0177777, 000000, "halt", do_halt},
        {0000000, 000000, "unknown command", do_nothing}
};

int main(int argc, char *argv[]) {
#ifdef TESTT
    test_mem(argv);
    run();
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
    return;
}
