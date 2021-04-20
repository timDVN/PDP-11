#include <assert.h>

void test_mem1() { //тест на моды, w/b_read/write (for file in1.txt)
    byte b0 = 0x0a;
    byte b1 = 0xcb;
    word w0 = 0xcb0a;
    b_write(15, b1);
    b_write(14, b0);
    w_write(12, w0);
    b_write(10, b0);
    printf("%06x\n", w_read(6));
    assert(b0 == b_read(10));
    assert(w0 == w_read(12));
    assert(w0 == w_read(14));
    assert(b0 == b_read(12));
    assert(b1 == b_read(13));
    for (int i = 0; i < 10; i+=2)
        printf("%06o ", w_read(i + 0x200));
    printf("\n\n\n");
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
    for (int i = 0; i < 8; i++) {
        printf("r%o = %o; ",i, reg[i]);
    }
    printf("\n");
}