void test4() //test for sob(with in4.txt file)
{
    w_write(0200,015);
    w_write(0202, 01);
    w_write(0204, -05);
    w_write(0206, 0250);
    reg[2] = 0200;
    reg[0] = 0;
    reg[1] = 4;
    run();
    assert(reg[0] == 0261);
}