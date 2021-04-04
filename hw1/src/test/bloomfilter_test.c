/* bloomfilter_test.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../../include/bloomfilter.h"

#define N 10000
#define P 0.1
#define LIMIT 100000
#define QUERIES 100

int main(void) {
    srand(time(NULL));

    BloomFilter *bf = blf_create(N, P);

    printf("Created Bloom Filter (M=%d, K=%d).\n",
           bf->m_bits, bf->k_hash);

    int x[N];
    int ba_size = ((int) ceil(((double) RAND_MAX + 1) / BAR_TYPE_BITS));
    BAR_TYPE *ba_x = (BAR_TYPE *) malloc(sizeof(BAR_TYPE) * ba_size);
    for (int i = 0; i < N; ++i) {
        int r = 0;
        while ((r = rand() % LIMIT) && TestBit(ba_x, r));
        x[i] = r;
        SetBit(ba_x, r);
    }

    for (int i = 0; i < N; ++i) {
        blf_add(bf, x[i]);
    }

    int y[N];
    BAR_TYPE *ba_y = (BAR_TYPE *) malloc(sizeof(BAR_TYPE) * ba_size);
    for (int i = 0; i < N; ++i) {
        int r = 0;
        while ((r = rand() % LIMIT) && TestBit(ba_y, r));
        y[i] = r;
        SetBit(ba_y, r);
    }

    int my = 0;
    int fp = 0;
    int no = 0;

    int true_yes = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (y[j] == x[i]) {
                true_yes++;
                break;
            }
        }
    }
    int true_no = N - true_yes;

    for (int i = 0; i < N; ++i) {
        if (blf_query(bf, y[i])) {
            my++;

            // Search y_i in x.
            int flag = 0;
            for (int j = 0; j < N; ++j) {
                if (y[i] == x[j]) {
                    flag = 1;
                    break;
                }
            }

            if (flag == 0) {
                fp++;
            }

        } else {
            no++;
        }
    }

    printf("TY: %d\n", true_yes);
    printf("TN: %d\n\n", true_no);

    printf("MY: %d\n", my);
    printf("NO: %d\n", no);
    printf("FP: %d\n\n", fp);

    printf("PER: %f\n", fp / (double) my);

    blf_destroy(&bf);

    free(ba_x);
    free(ba_y);

    return 0;
}
