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
    filter->m_bits = (int) ceil(-(n * log(p)) / (pow(log(2), 2)));
    filter->k_hash = (int) ceil(-log2(p));

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

//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    SHA256_CTX sha256;
//    SHA256_Init(&sha256);
//    SHA256_Update(&sha256, (unsigned char *) &x, sizeof(int));
//    SHA256_Final(hash, &sha256);
//
//    unsigned char hash2[SHA256_DIGEST_LENGTH];
//    SHA256_CTX sha256_2;
//    SHA256_Init(&sha256_2);
//    SHA256_Update(&sha256_2, (unsigned char *) &i, sizeof(int));
//    SHA256_Final(hash2, &sha256_2);
//
//    unsigned long result = 0;
//    unsigned long *h1 = (unsigned long *) hash;
//    unsigned long *h2 = (unsigned long *) hash2;
//    for (int j = 0; j < 4; ++j) {
//        result += h1[j] + h2[j];
//    }
//
//    return result;
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
