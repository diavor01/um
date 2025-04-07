#include "exec.h"
#include "opcode.h"
#include <uarray.h>

uint32_t registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
Seq_T recycled_ids;
Seq_T all_segments;

#define MOD_VAL (UINT64_C(1) << 32)

void print_segments() {
        for (int i = 0; i < Seq_length(all_segments); i++) {
                fprintf(stderr, "Segment %d:\n", i);
                void *elem = Seq_get(all_segments, i);
                fprintf(stderr, "Hey\n");
                if (elem != NULL) {
                        UArray_T curr_segment = (UArray_T)elem;
                        for (int j = 0; j < UArray_length(curr_segment); j++) {
                                print_binary(*(uint32_t*)UArray_at(curr_segment, j));
                        }
                } else {
                        fprintf(stderr, "NULL\n");
                }
        }
}

void print_registers() {
        fprintf(stderr, "registers: ");
        for (int i = 0; i < 8; i++) {
                fprintf(stderr, "%u ", registers[i]);
        }
        fprintf(stderr, "\n");
}

void print_recycled_ids()
{
        fprintf(stderr, "Printing recycled_ids\n");
        for (int i = 0; i < Seq_length(recycled_ids); i++) {
                fprintf(stderr, "%u ", *(uint32_t*)Seq_get(recycled_ids, i));
        }
        fprintf(stderr, "\n");
}

void initialize_program(UArray_T segment_zero)
{
        all_segments = Seq_new(10);
        Seq_addhi(all_segments, segment_zero);        
        recycled_ids = Seq_new(10);
}

void print_binary_char(char c)
{

        uint8_t val = 1 << 7;
        for (unsigned long i = 0; i < sizeof(c) * 8; i++)
        {
                uint32_t curr_dig = val & c;
                curr_dig = curr_dig >> 7;
                fprintf(stderr, "%u", curr_dig);
                c = c << 1;
        }
        fprintf(stderr, "\n");
}

void print_binary(uint32_t n)
{

        uint32_t val = 1 << 31;
        for (uint32_t i = 0; i < sizeof(n) * 8; i++)
        {
                uint32_t curr_dig = val & n;
                curr_dig = curr_dig >> 31;
                fprintf(stderr, "%u", curr_dig);
                n = n << 1;
        }
        fprintf(stderr, "\n");
}

// uint32_t read_instruction(FILE *fp)
// {
//         uint64_t instruction = 0;
//         for (int i = 0; i < 4; i++) {
//                 uint64_t c = (uint64_t)fgetc(fp);
//                 if ((int)c == EOF) {
//                         return (uint32_t)EOF;
//                 }
//                 instruction = Bitpack_newu(instruction, 8, 24 - i * 8, c);
//         }
//         // fprintf(stderr, "instruction: %lu\n", instruction);
//         return (uint32_t)instruction;

// }

// Seq_T store_code(FILE *fp)
// {
//         Seq_T segment_zero = Seq_new(100);
//         /*char c = getchar();
//         printf("first byte is %c\n", c);
//         print_binary_char(c);*/

//         uint32_t instruction = read_instruction(fp);
//         int counter = 0;

//         while ((int)instruction != EOF) {
//                 uint32_t *instruction_copy = malloc(sizeof(*instruction_copy));
//                 *instruction_copy = instruction;
//                 Seq_addhi(segment_zero, instruction_copy);
//                 print_binary(*(uint32_t*)Seq_get(segment_zero, counter++));
//                 instruction = read_instruction(fp);
//         }

//         return segment_zero;
// }

void execute_LV(uint8_t A, uint32_t val)
{
        printf("Inside loadval\n");
        registers[A] = val;
}

// void decode_instruction(uint32_t instruction, uint8_t *A, uint8_t *B, uint8_t *C, uint8_t *opcode, uint32_t *val)
// {
//         *opcode = (uint32_t)Bitpack_getu((uint64_t)instruction, 4, 28);
//         fprintf(stderr, "opcode is %u\n", *opcode);

//         if (*opcode != LV) {
//                 *A = (uint8_t)Bitpack_getu(instruction, 3, 6);
//                 *B = (uint8_t)Bitpack_getu(instruction, 3, 3);
//                 *C = (uint8_t)Bitpack_getu(instruction, 3, 0);
//         } else {
//                 *A = (uint8_t)Bitpack_getu(instruction, 3, 25);
//                 *val = (uint8_t)Bitpack_getu(instruction, 25, 0);
//         }

// }

void execute_CMOV(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside cmov\n");
        if (registers[C] != 0)
        {
                registers[A] = registers[B];
        }
}

void execute_SLOAD(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside sload\n");
        UArray_T curr_segment = (UArray_T)Seq_get(all_segments, registers[B]);
        registers[A] = *(uint32_t *)UArray_at(curr_segment, registers[C]);
        //fprintf(stderr, "ra: %u", registers[A]);
}

void execute_SSTORE(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside sstore\n");
        UArray_T curr_segment = (UArray_T)Seq_get(all_segments, registers[A]);
        uint32_t *spot = (uint32_t *)UArray_at(curr_segment, registers[B]);
        *spot = registers[C];

        // fprintf(stderr, "after SSTORE: ");
        // print_segments();
        // print_registers();
}

void free_recycled_ids()
{
        for (int i = 0; i < Seq_length(recycled_ids); i++) {
                free(Seq_get(recycled_ids, i));
        }
        
        Seq_free(&recycled_ids);
}

void execute_HALT()
{
        printf("Inside halt\n");
        // free memory
        for (int i = 0; i < Seq_length(all_segments); i++)
        {
                void *elem = Seq_get(all_segments, i);
                if (elem != NULL) {
                        //fprintf(stderr, "element %d is not NULL\n", i);
                        UArray_T curr_segment = (UArray_T)elem;
                        UArray_free(&curr_segment);
                } else {
                        //fprintf(stderr, "element %d is NULL\n", i);
                }
        }
        Seq_free(&all_segments);

        free_recycled_ids();
        exit(0);
}

void execute_OUT(uint8_t C)
{
        printf("Inside out\n");
        assert(registers[C] <= 255);
        printf("%c", (char)registers[C]);
        //print_registers();
}

void execute_INPUT(uint8_t C)
{
        printf("Inside input\n");
        int c = getchar();
        if (c == -1) {
                execute_LV(C, (unsigned)(0xFFFFFFFF));
                return;
        } 
        assert(c <= 255);
        execute_LV(C, (unsigned)c);

}

void execute_ADD(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside add\n");
        registers[A] = (registers[B] + registers[C]) % MOD_VAL;
}

void execute_MUL(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside mul\n");
        registers[A] = (registers[B] * registers[C]) % MOD_VAL;
}

void execute_DIV(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside div\n");
        registers[A] = registers[B] / registers[C];
}

void execute_NAND(uint8_t A, uint8_t B, uint8_t C)
{
        printf("Inside nand\n");
        registers[A] = ~(registers[B] & registers[C]);
}

void execute_ACTIVATE(uint8_t B, uint8_t C)
{
        printf("Inside activate\n");
        //fprintf(stderr, "before activating:");
        //print_segments();
        //fprintf(stderr, "starting activate\n");
        // print_recycled_ids();
        UArray_T new_seg = UArray_new(registers[C], 4);
        int id;
        if (Seq_length(recycled_ids) != 0) {
                int *id_pointer = (int*)Seq_remlo(recycled_ids);
                id = *id_pointer;
                // fprintf(stderr, "non-empty, id is %d\n", id);
                //UArray_T prev_segment = (UArray_T)Seq_put(all_segments, id, new_seg);
                //UArray_free(&prev_segment);
                Seq_put(all_segments, id, new_seg);
                // fprintf(stderr, "made it here\n");
                free(id_pointer);
        } else {
                id = Seq_length(all_segments);
                // fprintf(stderr, "empty, id is %d\n", id);
                Seq_addhi(all_segments, new_seg);
        }
        
        registers[B] = id;

        // fprintf(stderr, "after activating:\n");
//         print_segments();
//         print_registers();
}

void execute_INACTIVATE(uint8_t C) 
{
        printf("Inside inactivate\n");
        assert(registers[C] != 0);

        uint32_t *id = malloc(sizeof(*id));
        *id = registers[C];
        UArray_T curr_segment = (UArray_T)Seq_get(all_segments, *id);
        UArray_free(&curr_segment);

        Seq_put(all_segments, *id, (void*)NULL);

        //fprintf(stderr, "id in inactivate is %u", *id);
        Seq_addhi(recycled_ids, id);

        //fprintf(stderr, "after inactivating:\n");
        // print_segments();
        // print_registers();

}

UArray_T execute_LOADP(uint8_t B, uint8_t C, int *new_pc)
{
        printf("Inside laodp\n");
        *new_pc = registers[C];
        if (registers[B] == 0) {
                //fprintf(stderr, "registers[B] is 0, returning NULL\n");
                return NULL;
        }

        UArray_T segment_to_copy = Seq_get(all_segments, registers[B]);
        UArray_T new_segment = UArray_copy(segment_to_copy, UArray_length(segment_to_copy));
        UArray_T old = Seq_put(all_segments, 0, new_segment);
        UArray_free(&old); 

        //print_segments();

        return new_segment;
}

// uint64_t handle_instruction(uint32_t instruction, Seq_T all_segments)
// {

//         uint8_t A = 0;
//         uint8_t B = 0;
//         uint8_t C = 0;
//         uint32_t val = 0;
//         uint8_t opcode = 0;

//         decode_instruction(instruction, &A, &B, &C, &opcode, &val);

//         uint64_t x = 1 << 32;

//         switch (opcode) {
//                 case CMOV:
//                         execute_CMOV(A, B, C);
//                         return -1;
//                 case SLOAD:
//                         execute_SLOAD(A, B, C, all_segments);
//                         return -1;
//                 case SSTORE: return -1;
//                 case ADD: return -1;
//                 case MUL: return -1;
//                 case DIV: return -1;
//                 case NAND: return -1;
//                 case HALT:
//                         execute_HALT(all_segments);
//                         return -1;
//                 case ACTIVATE: return -1;
//                 case INACTIVATE: return -1;
//                 case OUT:
//                         execute_OUT(C);
//                         return -1;
//                 case IN: return -1;
//                 case LOADP:
//                         // call func
//                         return 0; // CHANGE
//                 case LV:
//                         execute_LV(A, val);
//                         return -1;
//                 default:
//                         return -1;
//         }
//         printf("We got here, although we shoul not\n");
// }
