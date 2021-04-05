/* bloomfilter.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef HW_SYSPRO_BLOOMFILTER_H
#define HW_SYSPRO_BLOOMFILTER_H

#include "bitarray.h"


/* ---------------------------- Data Types ---------------------------------- */

typedef struct {
    int m_bits;         // Number of bits in bit array.
    int k_hash;         // Number of hash functions.
    BAR_TYPE *array;    // Bit array.
} BloomFilter;

/* -------------------------- Basic Operations ------------------------------ */

BloomFilter *blf_create(unsigned int n, double p);

void blf_add(BloomFilter *filter, int x);

int blf_query(BloomFilter *filter, int x);

void blf_destroy(BloomFilter **filter);


/* --------------------------- Hash Functions ------------------------------- */

unsigned long hash_i(unsigned int x, unsigned int i);

unsigned long djb2(unsigned char *str);

unsigned long sdbm(unsigned char *str);


#endif //HW_SYSPRO_BLOOMFILTER_H
