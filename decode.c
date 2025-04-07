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

UArray_T handle_instruction(uint32_t instruction, int *pc)
{
        uint8_t A = 0;
        uint8_t B = 0;
        uint8_t C = 0;
        uint32_t val = 0;
        uint8_t opcode = 0;

        decode_instruction(instruction, &A, &B, &C, &opcode, &val);

        (*pc)++;

        switch (opcode) {
                case CMOV: 
                        execute_CMOV(A, B, C); 
                        return NULL;
                case SLOAD: 
                        execute_SLOAD(A, B, C);
                        return NULL;
                case SSTORE:
                        execute_SSTORE(A, B, C);
                        return NULL;
                case ADD: 
                        execute_ADD(A, B, C);
                        return NULL;
                case MUL: 
                        execute_MUL(A, B, C);
                        return NULL;
                case DIV: 
                        execute_DIV(A, B, C);
                        return NULL;
                case NAND:
                        execute_NAND(A, B, C); 
                        return NULL;
                case HALT: 
                        execute_HALT();
                        return NULL;
                case ACTIVATE: 
                        execute_ACTIVATE(B, C);
                        return NULL;
                case INACTIVATE: 
                        execute_INACTIVATE(C);
                        return NULL;
                case OUT:
                        execute_OUT(C); 
                        return NULL;
                case IN: 
                        execute_INPUT(C);
                        return NULL;
                case LOADP: 
                        return execute_LOADP(B, C, pc);
                case LV:
                        execute_LV(A, val);
                        return NULL;
                default:
                        exit(1);
        }
        //printf("We got here, although we shoul not\n");
}