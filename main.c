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
                //fprintf(stderr, "pc is %d\n", pc);
                uint32_t instruction = *(uint32_t*)UArray_at(segment_zero, pc);

                //fprintf(stderr, "Printing segment_zero\n");
                // for (int i = 0; i < UArray_length(segment_zero); i++)
                // {
                //         fprintf(stderr, "%u ", *(uint32_t*)UArray_at(segment_zero, i));
                // }
                // fprintf(stderr, "\n");
                
                
                UArray_T new_segment_zero = handle_instruction(instruction, &pc);

                if (new_segment_zero != NULL) {
                        //fprintf(stderr, "Got here\n");
                        segment_zero = new_segment_zero;
                }

        }
        // fprintf(stderr, "After loop: Printing segment_zero\n");
        // for (int i = 0; i < UArray_length(segment_zero); i++)
        // {
        //         fprintf(stderr, "%u ", *(uint32_t*)UArray_at(segment_zero, i));
        // }
        // fprintf(stderr, "\n");

        /*printf("After assigning the instructions\n");
        for (int i = 0; i < Seq_length(segment_zero); i++) {
                
                printf("%d ", *(int*)Seq_get(segment_zero, i));
        }*/
        //printf("\n");
}