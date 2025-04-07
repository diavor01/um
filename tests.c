/*
* umlab.c
*
* Functions to generate UM unit tests. Once complete, this module
* should be augmented and then linked against umlabwrite.c to produce
* a unit test writing program.
*  
* A unit test is a stream of UM instructions, represented as a Hanson
* Seq_T of 32-bit words adhering to the UM's instruction format.  
* 
* Any additional functions and unit tests written for the lab go
* here. 
*  
*/


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc) {
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 6, ra);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 0, rc);

        return word;
}

Um_instruction loadval(unsigned ra, unsigned val) {
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, LV);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);

        return word;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction cmov(Um_register a, Um_register b, Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mul(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction sstore(Um_register a, Um_register b, Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction sload(Um_register a, Um_register b, Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction activate(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction inactivate(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

Um_instruction out(Um_register c) {
        return three_register(OUT, 0, 0, c);
}

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
    
}


/* Unit tests for the UM */

void build_halt_test1(Seq_T stream)
{
        append(stream, halt());
}

/* basic out/loadval test */
void build_out_test1(Seq_T stream)
{
        append(stream, loadval(r0, 65));
        append(stream, out(0));
        append(stream, halt());
}

/* test loading, overwriting, and outputting all 8 registers with ints */
void build_out_test2(Seq_T stream) 
{
        append(stream, loadval(r6, 10)); // new line character
        append(stream, loadval(r7, 32)); // space character

        for (int i = 0; i < 6; i++) { 
                append(stream, loadval(i, 65 + i)); 
        }
        
        for (int i = 0; i < 6; i++) {
                append(stream, out(i));
                append(stream, out(r7));
        }

        append(stream, out(6));

        for (int i = 0; i < 6; i++) { 
                append(stream, loadval(i, 66 + i));
        };

        for (int i = 0; i < 6; i++) {
                append(stream, out(i));
                append(stream, out(r7));
        }

        append(stream, out(6));

        append(stream, halt());
                
}

/* test loading with a non-standard character */
void build_out_test3(Seq_T stream)
{
        append(stream, loadval(r7, 243));
        append(stream, out(r7));
        append(stream, halt());
}


/* test inputting intothe same register and overwriting */
void build_input_test1(Seq_T stream)
{
        append(stream, input(r0));
        append(stream, out(r0));
        append(stream, input(r0));
        append(stream, out(r0));
        append(stream, input(r0));
        append(stream, out(r0));
        append(stream, input(r0));
        append(stream, out(r0));
        append(stream, input(r0));
        append(stream, out(r0));
        /* EOF */
        append(stream, input(r0));
        append(stream, halt());
}

/* test inputting into different registers */
void build_input_test2(Seq_T stream)
{
        append(stream, input(r0));  
        append(stream, input(r1)); 
        append(stream, input(r2)); 
        append(stream, input(r3)); 
        append(stream, out(r0)); 
        append(stream, out(r1)); 
        append(stream, out(r2)); 
        append(stream, out(r3)); 
        append(stream, input(r0));
        append(stream, input(r4)); /* EOF */
        append(stream, out(r0)); 
        append(stream, halt());

}

/* test case where registers[C] is 0, so no move is made*/
void build_cmov_test1(Seq_T stream)
{
        append(stream, loadval(r0, 0)); 
        append(stream, loadval(r1, 50)); 
        append(stream, loadval(r2, 70)); 
        append(stream, cmov(r1, r2, r0)); 
        append(stream, out(r1)); 
        append(stream, out(r2));
        append(stream, halt());  

}

/* test case where r[C] is not zero, so the move happens */
void build_cmov_test2(Seq_T stream)
{
        append(stream, loadval(r0, 1)); 
        append(stream, loadval(r1, 50)); 
        append(stream, loadval(r2, 70)); 
        append(stream, cmov(r1, r2, r0)); 
        append(stream, out(r1)); 
        append(stream, out(r2));
        append(stream, halt());  

}

/* basic add test */
void build_add_test1(Seq_T stream) 
{
        append(stream, loadval(r0, 67)); 
        append(stream, loadval(r1, 12)); 
        append(stream, loadval(r2, 9)); 
        append(stream, out(r0));
        append(stream, add(r3, r0, r1)); 
        append(stream, out(r3)); 
        append(stream, out(r3)); 
        append(stream, add(r4, r0, r2)); 
        append(stream, out(r4)); 
        append(stream, halt());
}

/* tests adding a register value to itself, storing in the same register */
void build_add_test2(Seq_T stream) 
{
        append(stream, loadval(r0, 36)); 
        append(stream, add(r0, r0, r0));
        append(stream, out(r0));
        append(stream, halt());
}

/* tests overwriting register with result */
void build_mul_test1(Seq_T stream)
{
        append(stream, loadval(r0, 33));
        append(stream, loadval(r5, 2));
        append(stream, mul(r4, r0, r5));
        append(stream, out(r4));
        append(stream, halt());
}

/* tests case where result is above 2^32, program must mod */
void build_mul_test2(Seq_T stream)
{
        /* 2^24 */
        uint32_t x = 1 << 24;
        /* 2^8 */
        uint32_t y = 1 << 8;
        
        append(stream, loadval(r0, x));
        append(stream, loadval(r5, y));
        append(stream, loadval(r3, 65));
        append(stream, mul(r4, r0, r5)); /* result: 2^32 = 0 */
        append(stream, add(r2, r4, r3));
        append(stream, out(r2));
        append(stream, halt());
}

/* test case where rounding down is required */
void build_div_test1(Seq_T stream)
{
        append(stream, loadval(r0, 131)); 
        append(stream, loadval(r1, 2)); 
        append(stream, div(r1, r0, r1)); 
        append(stream, out(r1)); 
        append(stream, halt());
}

void build_div_test2(Seq_T stream)
{
        append(stream, loadval(r0, 122)); 
        append(stream, out(r0)); 
        append(stream, loadval(r1, r2)); 
        append(stream, div(r2, r0, r1)); 
        append(stream, out(r2)); 
        append(stream, loadval(r5, 29)); 
        append(stream, add(r4, r2, r5)); 
        append(stream, out(r4)); 
        append(stream, halt()); 
}

void build_nand_test1(Seq_T stream)
{
        append(stream, loadval(r0, 209)); 
        append(stream, loadval(r1, 17)); 
        append(stream, loadval(r2, 127)); 
        append(stream, loadval(r3, 122)); 
        append(stream, nand(r4, r0, r1)); 
        append(stream, nand(r5, r2, r3)); 
        append(stream, nand(r6, r4, r5)); 
        append(stream, out(r6));
        append(stream, halt());
}

void build_activate_test1(Seq_T stream)
{
        append(stream, loadval(r4, 3));
        append(stream, activate(r2, r4)); 
        append(stream, halt());

}

void build_inactivate_test1(Seq_T stream)
{
        append(stream, loadval(r4, 3));
        append(stream, activate(r2, r4)); 
        append(stream, inactivate(r2)); 
        append(stream, halt());

}

void build_inactivate_test2(Seq_T stream) 
{
        append(stream, activate(r0, r3)); 
        append(stream, activate(r1, r4));
        append(stream, inactivate(r0)); 
        append(stream, activate(r2, r5)); 
        append(stream, halt());

}

void build_sstore_test1(Seq_T stream)
{
        append(stream, loadval(r4, 4)); /* load length of new segment */
        append(stream, activate(r0, r4)); /* segment id goes into r0 */
        append(stream, loadval(r3, 8)); /* load value to insert */
        append(stream, loadval(r2, 2)); /* load index to insert at */
        append(stream, sstore(r0, r2, r3)); 
        append(stream, halt());

}

void build_sload_test1(Seq_T stream)
{
        append(stream, loadval(r4, 5)); /* load length of new segment */
        append(stream, activate(r0, r4)); /* segment id goes into r0 */
        append(stream, loadval(r3, 65)); /* load value to insert */
        append(stream, loadval(r2, 3)); /* load index to insert at */
        append(stream, sstore(r0, r2, r3));
        append(stream, sload(r4, r0, r2));
        append(stream, out(r4));
        //append(stream, halt());
}


// void built_LV_test(Seq_T stream)
// {
//         append(stream, loadval(r0, 300));
//         append(stream, output(r0));
// }

// void build_cmov_test(Seq_T stream)
// {
//         append(stream, loadval(r0, 12));
//         append(stream, cmov(r0, r1, r2));
//         append(stream, halt());
// }

// void build_verbose_halt_test(Seq_T stream)
// {
//         append(stream, loadval(r1, '\n'));
//         append(stream, output(r1));
//         append(stream, loadval(r1, '9'));
//         append(stream, output(r1));
//         append(stream, loadval(r1, ' '));
//         append(stream, output(r1));
//         append(stream, loadval(r1, '\0'));
//         append(stream, output(r1));
//         append(stream, loadval(r1, 'p'));
//         append(stream, output(r1));
//         append(stream, halt());
// }

/*
void build_add_test(Seq_T stream)
{
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}


void build_add_output_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        
        append(stream, halt());
}*/

