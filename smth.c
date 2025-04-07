#include "seq.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
        int x = 5;
        int y = 6;
        int z = 7;
        Seq_T seq1 = Seq_seq((void*)&x, (void*)&y, (void*)&z);
        Seq_T seq2 = Seq_seq((void*)&y, (void*)&z, (void*)&x);

        // seq2 = seq1;

        for(int i = 0; i < 3; i++) {
                printf("%d", *(int*)Seq_get(seq1, i));
        }

        for(int i = 0; i < 3; i++) {
                printf("%d", *(int*)Seq_get(seq2, i));
        }
}