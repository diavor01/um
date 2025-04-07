#ifndef OPCODEH
#define OPCODE_H

typedef enum Um_opcode
{
        CMOV = 0,
        SLOAD,
        SSTORE,
        ADD,
        MUL,
        DIV,
        NAND,
        HALT,
        ACTIVATE,
        INACTIVATE,
        OUT,
        IN,
        LOADP,
        LV
} Um_opcode;

#endif