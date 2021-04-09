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

#include "../include/hash.h"
#include "../include/bloomfilter.h"


/* -------------------------- Basic Operations ------------------------------ */

BloomFilter *blf_create(unsigned int n, unsigned int m) {
    /* Create a Bloom Filter given the (n) estimated elements to be inserted and
     * (p) the desired probability of false positives. */

    BloomFilter *filter = (BloomFilter *) malloc(sizeof(BloomFilter));

    // Calculate the required m bits and k hash functions.
    filter->m_bits = m;
    filter->k_hash = (int) round((m /(double ) n) * log(2));


//    filter->m_bits = (int) (ceil(-(n * log(p)) / (pow(log(2), 2))));
//    filter->k_hash = (int) round(-log2(p));

    // Create and initialize bit array.
    int size = (int) ceil(filter->m_bits / (double) BAR_TYPE_BITS);
    filter->array = (BAR_TYPE *) calloc(size, sizeof(BAR_TYPE));

    return filter;
}

void blf_add(BloomFilter *filter, int x) {
    for (int k = 0; k < filter->k_hash; ++k) {
        SetBit(filter->array,
               (hash_i((unsigned char *) &x, k) % filter->m_bits));
    }
}

int blf_query(BloomFilter *filter, int x) {
    int k = 0;
    while ((k < filter->k_hash) &&
           (TestBit(filter->array,
                    (hash_i((unsigned char *) &x, k) % filter->m_bits)))) {
        k++;
    }
    return ((k < filter->k_hash) ? 0 : 1);
}

void blf_destroy(BloomFilter **filter) {
    free((*filter)->array);
    free((*filter));
    *filter = NULL;
}
