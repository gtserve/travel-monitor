#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../../include/skiplist.h"

#define NUMBERS 25
#define RANGE 100


int main(void) {
    srand(time(NULL));

    SkipList *sl = skl_create();

    int x[NUMBERS];
    for (int i = 0; i < NUMBERS; ++i) {
        x[i] = rand() % RANGE;
    }

    for (int i = 0; i < NUMBERS; ++i) {
        skl_insert(sl, x[i]);
    }
    skl_print(sl);
    printf("\n");


    for (int i = 0; i < NUMBERS / 4; i++) {
        int j = rand() % NUMBERS;
        printf("Deleting item [%d].\n", x[j]);
        skl_delete(sl, x[j]);
    }
    skl_print(sl);
    printf("\n");

    skl_destroy(&sl);
    return 0;
}
