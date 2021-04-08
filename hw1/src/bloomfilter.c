/* bloomfilter.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "../include/bloomfilter.h"


/* -------------------------- Basic Operations ------------------------------ */

BloomFilter *blf_create(unsigned int n, double p) {
    /* Create a Bloom Filter given the (n) estimated elements to be inserted and
     * (p) the desired probability of false positives. */

    BloomFilter *filter = (BloomFilter *) malloc(sizeof(BloomFilter));

    // Calculate the required m bits and k hash functions.
    filter->m_bits = (int) (ceil(-(n * log(p)) / (pow(log(2), 2))));
    filter->k_hash = (int) round(-log2(p));

    // Create and initialize bit array.
    int size = (int) ceil(filter->m_bits / (double) BAR_TYPE_BITS);
    filter->array = (BAR_TYPE *) calloc(size, sizeof(BAR_TYPE));

    return filter;
}

void blf_add(BloomFilter *filter, int x) {
    for (int k = 0; k < filter->k_hash; ++k) {
        SetBit(filter->array, (hash_i(x, k) % filter->m_bits));
    }
}

int blf_query(BloomFilter *filter, int x) {
    int k = 0;
    while ((k < filter->k_hash) &&
           (TestBit(filter->array, (hash_i(x, k) % filter->m_bits)))) {
        k++;
    }
    return ((k < filter->k_hash) ? 0 : 1);
}

void blf_destroy(BloomFilter **filter) {
    free((*filter)->array);
    free((*filter));
    *filter = NULL;
}


/* --------------------------- Hash Functions ------------------------------- */

unsigned long hash_i(unsigned int x, unsigned int i) {
    /* Only this one was changed from the original file h39jdk2.c to function
     * also as a wrapper and support integers. */

    return (djb2((unsigned char *) &x) + i * sdbm((unsigned char *) &x) + i * i);
}

unsigned long djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

unsigned long sdbm(unsigned char *str) {
    unsigned long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
