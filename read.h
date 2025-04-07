#ifndef READ_H
#define READ_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <bitpack.h>
#include "uarray.h"

UArray_T store_code(FILE *fp, long long num_instructions);

#endif