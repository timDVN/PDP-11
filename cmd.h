#include <stdio.h>
#include "do_func.h"
typedef unsigned short int word;// 16 bit

typedef struct {
    word mask;
    word  opcode;
    char * name;
    void (do_func) (void );
}Command;

