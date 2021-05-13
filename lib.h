#define MEMSIZE (64*1024)
typedef unsigned char byte;// 8 bit
typedef unsigned short int word;// 16 bit
typedef word Adress;

typedef struct arg {
    word val; // значние аргумента
    word adr; // адрес аргумента
} Arg;

typedef struct command {
    unsigned short int mask;
    unsigned short int opcode;
    char *name;
    char params;

    void (*func)(void);
} Command;

extern Arg ss;
extern Arg dd;
extern word N;
extern word R;
extern word X;
extern Command cmd[];
extern byte flag_N;
extern byte flag_Z;
extern byte flag_C;
extern unsigned char mem[64 * 1024];
extern word reg[8];
extern byte B;
extern word dif;
#define pc reg[7]

word w_read(unsigned short int adr);

byte b_read(unsigned short int adr);

word byte_to_word(byte b);

void b_write(Adress adr, byte b);

void w_write(Adress adr, word w);

void load_file(const char *file);


//
void print_flags();

void set_N(word w);

void set_C(int w);

void set_Z(word w);

void print_r();

void trace_func(const char *format, ...);
//


//
        Arg get_mr(word w);
//


//
        void do_adc();

        void do_add();

        void do_move();

        void do_nothing();

        void do_br();

        void do_beq();

        void do_bpl();

        void do_halt();

        void do_ash();

        void do_ashc();

        void do_asl();

        void do_asr();

        void do_tst();

        void do_cmp();

        void do_com();

        void do_jsr();

        void do_rts();

        void do_rol();

        void do_ror();

        void do_div();

        void do_sob();

        void do_clr();

        void do_scc();

        void do_sec();

        void do_sen();

        void do_sez();

        void do_ccc();

        void do_clc();

        void do_cln();

        void do_clz();
//


//
        void run();
//
