extern byte flag_N;
extern byte flag_Z;
extern byte flag_C;

void set_N(word w)
{
    if (((w >> 15) & 1) != 0)
        flag_N = 1;
    else
        flag_N = 0;
}

void set_C(int w)
{
    if ((((w >> 16) & 1) == 1) && (w > 0))
        flag_C = 1;
    else
        flag_C = 0;
}

void set_Z(word w)
{
    if (w == 0)
        flag_Z = 1;
    else
        flag_Z = 0;

}