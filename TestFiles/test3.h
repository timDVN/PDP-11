void test3(){ //tests for asl,asr,br (for file in3.txt)
    reg[1] = 010;
    reg[2] = 01050;
    reg[3] = 01050;
    reg[4] = 1050;
    run();
    assert(reg[1] == 020);
    assert(reg[2] == 0x450);
    assert(reg[3] == 0x50);
    assert(reg[4] == 0x14);
    assert(reg[7] == 01034);

}