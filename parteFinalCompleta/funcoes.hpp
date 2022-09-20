
#define MAXLEN 63

typedef enum {

    OP_PRINT = 0,
    OP_LOAD,
    OP_STORE,
    OP_JUMP,
    OP_MOV,

    OP_IN,
    OP_OUT,

    OP_IRET,


    OP_ADD,
    OP_SUB,


    OP_CONCAT,
    OP_CLR,
    OP_JZ,
    OP_JL,
    OP_JH,
} opcode_t;

typedef struct {
    opcode_t opcode;
    int operand;
} instruction_t;

typedef union {
    instruction_t code;
    char data[MAXLEN + 1];
} memdata_t;

typedef enum {
    INT_ZERO = 0,
    INT_DISK = 1
} interruption_t;

typedef enum {
    PORT_DISK_CMD,
    PORT_DISK_STATUS,
    PORT_DISK_DATA,
    PORT_DMA_ADDR
} port_t;
