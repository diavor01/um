#include "exec.h"
#include "read.h"
#include "decode.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL) {
                fprintf(stderr, "%s: No such file or directory", argv[1]);
                exit(EXIT_FAILURE);
        }

        struct stat st;
        assert(stat(argv[1], &st) == 0);
        long long num_instructions = st.st_size / 4;
        //printf("size is %lld\n", num_instructions);

        UArray_T segment_zero = store_code(fp, num_instructions);
        // Seq_T all_segments = Seq_new(10);
        // Seq_addhi(all_segments, segment_zero);
        initialize_program(segment_zero);

        int pc = 0;
        while (pc < UArray_length(segment_zero)) {
                // decode
                uint32_t instruction = *(uint32_t*)UArray_at(segment_zero, pc);
                //fprintf(stderr, "instruction is %u\n", instruction);
                //print_binary(instruction);

                int64_t new_pc = handle_instruction(instruction);
                
                // //Change it to -1 vs other value
                if (new_pc != NULL) {
                        segment_zero = new_pc;
                } else {
                        pc++;
                }
                //pc++;
                // (void)is_loadprogram;

                //fprintf(stderr, "Counter: %d\n\n", pc);
                /*if (opcode == 13) {
                        // load val
                } else {
                        // three registers - inside here, do switch
                        
                }*/
                //decode_instruction(instruction, );

        }
        execute_HALT();
        /*printf("After assigning the instructions\n");
        for (int i = 0; i < Seq_length(segment_zero); i++) {
                
                printf("%d ", *(int*)Seq_get(segment_zero, i));
        }*/
        //printf("\n");
}