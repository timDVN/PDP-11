

typedef struct command {
    unsigned short int mask;
    unsigned short int opcode;
    char *name;
    char  params;
    void (*func) (void);
} Command;