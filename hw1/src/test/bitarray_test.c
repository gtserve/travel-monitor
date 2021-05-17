
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../../data-structures/hash-table/bitarray.h"

#define TESTS 8
#define SIZE 10


int main(void) {
    srand(time(0));

    BAR_TYPE A[SIZE] = {0};
    int array_size = SIZE * BAR_TYPE_BITS;

    int x[TESTS];
    for (int i = 0; i < TESTS; ++i) {
        x[i] = i;
    }

    for (int i = 0; i < TESTS; i++) {
        SetBit(A, x[i]);
    }

    for (int i = 0; i < array_size; ++i) {
        if (TestBit(A, i)) {
            printf("A[%d] is set!\n", i);
        }
    }

    printf("\nBitArray:\n");
    for (int i = 0; i < array_size; ++i) {
        if (TestBit(A, i)) {
            printf("(1) ");
        } else {
            printf("0 ");
        }
        if ((i + 1) % BAR_TYPE_BITS == 0) {
            printf("\n");
        }
    }
    printf("\n");

    for (int i = 0; i < TESTS; i++) {
        ClearBit(A, x[i]);
        if (TestBit(A, x[i])) {
            printf("A[%d] is still set!\n", x[i]);
        } else {
            printf("A[%d] has been cleared!\n", x[i]);
        }
    }

    return 0;
}

