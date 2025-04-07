#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

void decode_instruction(uint32_t instruction, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *opcode, uint32_t *val);
uint64_t handle_instruction(uint32_t instruction);

#endif