#include <stdio.h>

typedef unsigned short int word;// 16 bit

typedef struct command {
    word mask;
    word opcode;
    char *name;

    void (*func)(struct command *self);
} Command;