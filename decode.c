#include "decode.h"
#include "exec.h"
#include "opcode.h"



void decode_instruction(uint32_t instruction, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *opcode, uint32_t *val)
{
        *opcode = (uint32_t)Bitpack_getu((uint64_t)instruction, 4, 28);
        //fprintf(stderr, "opcode is %u\n", *opcode);

        if (*opcode != LV) {
                *A = (uint8_t)Bitpack_getu(instruction, 3, 6);
                *B = (uint8_t)Bitpack_getu(instruction, 3, 3);
                *C = (uint8_t)Bitpack_getu(instruction, 3, 0);
        } else {
                *A = (uint8_t)Bitpack_getu(instruction, 3, 25);
                *val = (uint32_t)Bitpack_getu(instruction, 25, 0);
        }

        
}

uint64_t handle_instruction(uint32_t instruction,)
{
        uint8_t A = 0;
        uint8_t B = 0;
        uint8_t C = 0;
        uint32_t val = 0;
        uint8_t opcode = 0;

        decode_instruction(instruction, &A, &B, &C, &opcode, &val);
        
        switch (opcode) {
                case CMOV: 
                        execute_CMOV(A, B, C); 
                        return -1;
                case SLOAD: 
                        execute_SLOAD(A, B, C);
                        return -1;
                case SSTORE:
                        execute_SSTORE(A, B, C);
                        return -1;
                case ADD: 
                        execute_ADD(A, B, C);
                        return -1;
                case MUL: 
                        execute_MUL(A, B, C);
                        return -1;
                case DIV: 
                        execute_DIV(A, B, C);
                        return -1;
                case NAND:
                        execute_NAND(A, B, C); 
                        return -1;
                case HALT: 
                        execute_HALT();
                        return -1;
                case ACTIVATE: 
                        execute_ACTIVATE(B, C);
                        return -1;
                case INACTIVATE: 
                        execute_INACTIVATE(C);
                        return -1;
                case OUT:
                        execute_OUT(C); 
                        return -1;
                case IN: 
                        execute_INPUT(C);
                        return -1;
                case LOADP: 
                        return execute_LOADP(B, C);
                        return 0; // CHANGE
                case LV:
                        execute_LV(A, val);
                        return -1;
                default:
                        exit(1);
        }
        printf("We got here, although we shoul not\n");
}