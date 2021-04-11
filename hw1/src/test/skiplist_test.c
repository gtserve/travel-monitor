
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../include/skiplist.h"

#define N 1000
#define LIMIT 10000


int main(void) {
    srand(time(NULL));

    SkipList *sl = skl_create();

    // Create array of number.
    int x[N];
    for (int i = 0; i < N; ++i) {
        x[i] = rand() % LIMIT;
    }
//    for (int i = 0; i < N; ++i) {
//        x[i] = i;
//    }

    // Insert number as keys and items.
    for (int i = 0; i < N; ++i) {
        skl_insert(sl, x[i], &x[i]);
    }

    // Search random positions in previous array.
    for (int i = 0; i < N; i++) {
        int *k = skl_search(sl, x[i]);
        if (!k)
            printf("KEY: %d NOT FOUND\n", x[i]);
    }

    skl_destroy(&sl);
    return 0;
}
