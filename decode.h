#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include <uarray.h>

void decode_instruction(uint32_t instruction, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *opcode, uint32_t *val);
UArray_T handle_instruction(uint32_t instruction, int *pc);

#endif