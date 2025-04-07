#include "read.h"
#include <uarray.h>


// void print_binary(uint32_t n)
// {

//         uint32_t val = 1 << 31;
//         for (uint32_t i = 0; i < sizeof(n) * 8; i++)
//         {
//                 uint32_t curr_dig = val & n;
//                 curr_dig = curr_dig >> 31;
//                 fprintf(stderr, "%u", curr_dig);
//                 n = n << 1;
//         }
//         fprintf(stderr, "\n");
// }

uint32_t read_instruction(FILE *fp)
{
        uint64_t instruction = 0;
        for (int i = 0; i < 4; i++) {
                uint64_t c = (uint64_t)fgetc(fp);
                if ((int)c == EOF) {
                        return (uint32_t)EOF;
                } 
                instruction = Bitpack_newu(instruction, 8, 24 - i * 8, c);
        }       
        // fprintf(stderr, "instruction: %lu\n", instruction);
        return (uint32_t)instruction;

}

UArray_T store_code(FILE *fp, long long num_instructions) 
{
        UArray_T segment_zero = UArray_new(num_instructions, 4);
        /*char c = getchar();
        printf("first byte is %c\n", c);
        print_binary_char(c);*/
        
        uint32_t instruction = read_instruction(fp);
        int counter = 0;

        while ((int)instruction != EOF) {
                //uint32_t *instruction_copy = malloc(sizeof(*instruction_copy));
                uint32_t instruction_copy = instruction;
                uint32_t *spot = (uint32_t*)UArray_at(segment_zero, counter);
                //Seq_addhi(segment_zero, instruction_copy);
                *spot = instruction_copy;
                //print_binary(*(uint32_t*)UArray_at(segment_zero, counter++));
                counter ++;
                instruction = read_instruction(fp);
        }
        
        fclose(fp);
        return segment_zero;
}