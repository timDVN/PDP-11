#include <assert.h>

void test2(){//tests for add (for file in2.txt)
    reg[0] = 5;
    reg[1] = 3;
    reg[2] = 01010;
    reg[3] = 1;
    reg[4] = 01000;
    run();
    assert(reg[0] == 010);
    assert(reg[3] == 01003);
    assert(w_read(reg[4]) == 060101);
    assert(reg[5] == 060101);
}