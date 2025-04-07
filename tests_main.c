#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *out, Seq_T instructions);

extern void build_halt_test1(Seq_T stream);
extern void build_out_test1(Seq_T stream);
extern void build_out_test2(Seq_T stream);
extern void build_out_test3(Seq_T stream);

// extern void built_LV_test(Seq_T stream);
// extern void build_cmov_test(Seq_T instructions);
// extern void build_verbose_halt_test(Seq_T instructions);
extern void build_input_test1(Seq_T stream);
extern void build_input_test2(Seq_T stream);
extern void build_cmov_test1(Seq_T stream);
extern void build_cmov_test2(Seq_T stream);
extern void build_add_test1(Seq_T stream);
extern void build_add_test2(Seq_T stream);
extern void build_mul_test1(Seq_T stream);
extern void build_mul_test2(Seq_T stream);
extern void build_div_test1(Seq_T stream);
extern void build_div_test2(Seq_T stream);
extern void build_nand_test1(Seq_T stream);
extern void build_activate_test1(Seq_T stream);
extern void build_inactivate_test1(Seq_T stream);
extern void build_inactivate_test2(Seq_T stream);
extern void build_sstore_test1(Seq_T stream);
extern void build_sload_test1(Seq_T stream);
extern void build_loadp_test1(Seq_T stream);
extern void build_loadp_test2(Seq_T stream);
/*
extern void build_add_test(Seq_T instructions);
extern void build_add_output_test(Seq_T instructions);
*/


/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        //{ "folder/halt1", NULL, "", build_halt_test1 },
        // { "folder/out1", NULL, "A", build_out_test1},
        //{"folder/out2", NULL, "A B C D E F \nB C D E F G \n", build_out_test2},
        //{ "folder/out3", NULL, "", build_out_test3 } // This test requires the special character,
        //{"folder/input1", "qwert", "qwert", build_input_test1},
        //{"folder/input2", "Hello", "Hello", build_input_test2},
        //{ "folder/cmov1", NULL, "FF", build_cmov_test1},
        //{ "folder/cmov2", NULL, "FF", build_cmov_test2 },
        //{ "folder/add1", NULL, "COOL", build_add_test1 }
        //{ "folder/add2", NULL, "H", build_add_test2 },
        //{ "folder/mul1", NULL, "B", build_mul_test1 },
        //{ "folder/mul2", NULL, "A", build_mul_test2 },
        //{"folder/div1", NULL, "A", build_div_test1},
        //{"folder/div2", NULL, "z=Z", build_div_test2},
        //{"folder/nand1", NULL, "{", build_nand_test1},
        //{"folder/activate1", NULL, "", build_activate_test1},
        //{"folder/inactivate1", NULL, "", build_inactivate_test1}
        //{"folder/inactivate2", NULL, "", build_inactivate_test2},
        //{"folder/sstore1", NULL, "", build_sstore_test1},
        //{"folder/sload1", NULL, "", build_sload_test1}
        //{"folder/loadp1", NULL, "", build_loadp_test1},
        {"folder/loadp2", NULL, "", build_loadp_test2}
};

#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
