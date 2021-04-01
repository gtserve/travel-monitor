
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/bit_array.h"

#define TESTS 5
#define SIZE 10


int main(void) {
    srand(time(0));

    NUM_TYPE A[SIZE] = {0};
    int array_size = SIZE * BIT_SIZE;



    int x[TESTS];
    for (int i = 0; i < TESTS; ++i) {
        x[i] = rand() % array_size;
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
            printf("1 ");
        } else {
            printf("0 ");
        }
        if ((i + 1) % BIT_SIZE == 0) {
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

