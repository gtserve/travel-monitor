/* bloomfilter_test.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../../include/hash.h"

#include "../../include/bloomfilter.h"

#define N 50
//#define P 0.6
#define M 1000
#define LIMIT 100000
#define QUERIES 100

unsigned long djb2_og(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

unsigned long sdbm_og(unsigned char *str) {
    unsigned long hash = 0;
    int c;

    while (c = *str++) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned long hash_i_og(unsigned char *str, unsigned int i) {
    return djb2_og(str) + i*sdbm_og(str) + i*i;
}


int main(void) {
    srand(time(0));

    BloomFilter *bf = blf_create(N, M);

    printf("Created Bloom Filter (M=%d, K=%d).\n",
           bf->m_bits, bf->k_hash);

    unsigned char str[30];
    for (int i = 1; i < 33; i++) {
        int num = 1000 + i;

    }

//    int ba_size = ((int) ceil(((double) RAND_MAX + 1) / BAR_TYPE_BITS));
//
//    int x[N];
//    BAR_TYPE *ba_x = (BAR_TYPE *) calloc(ba_size, sizeof(BAR_TYPE));
//    for (int i = 0; i < N; ++i) {
//        int r = 0;
//        while ((r = rand() % LIMIT) && TestBit(ba_x, r));
//        x[i] = r;
//        SetBit(ba_x, r);
//    }
//
//    for (int i = 0; i < N; ++i) {
//        blf_add(bf, x[i]);
//    }
//
//    int y[N];
//    BAR_TYPE *ba_y = (BAR_TYPE *) calloc(ba_size, sizeof(BAR_TYPE));
//    for (int i = 0; i < N; ++i) {
//        int r = 0;
//        while ((r = rand() % LIMIT) && TestBit(ba_y, r));
//        y[i] = r;
//        SetBit(ba_y, r);
//    }
//
//    int my = 0;
//    int fp = 0;
//    int no = 0;
//
//    int true_yes = 0;
//    for (int i = 0; i < N; ++i) {
//        for (int j = 0; j < N; ++j) {
//            if (y[i] == x[j]) {
//                true_yes++;
//                break;
//            }
//        }
//    }
//    int true_no = N - true_yes;
//
//    for (int i = 0; i < N; ++i) {
//        if (blf_query(bf, y[i])) {
//            my++;
//
//            // Search y_i in x.
//            int flag = 0;
//            for (int j = 0; j < N; ++j) {
//                if (y[i] == x[j]) {
//                    flag = 1;
//                    break;
//                }
//            }
//
//            if (flag == 0) {
//                fp++;
//            }
//
//        } else {
//            no++;
//        }
//    }
//
//    printf("TY: %d\n", true_yes);
//    printf("TN: %d\n\n", true_no);
//
//    printf("MY: %d\n", my);
//    printf("NO: %d\n", no);
//    printf("FP: %d\n\n", fp);
//
//    printf("FP RATE: %f\n", fp / (double) (fp + true_no));
//
//    blf_destroy(&bf);
//
//    free(ba_x);
//    free(ba_y);

    return 0;
}
