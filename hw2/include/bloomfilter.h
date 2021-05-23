/* bloomfilter.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#ifndef HW_SYSPRO_BLOOMFILTER_H
#define HW_SYSPRO_BLOOMFILTER_H

#include "bitarray.h"

#define BF_ARR_BYTES(bf) (((int) ceil((bf)->m_bits / (double) BAR_TYPE_BITS)) * sizeof(BAR_TYPE))

/* ---------------------------- Data Types ---------------------------------- */

typedef struct {
    unsigned int m_bits;         // Number of bits in bit array.
    unsigned int k_hash;         // Number of hash functions.
    BAR_TYPE *array;             // Bit array.
} BloomFilter;

/* -------------------------- Basic Operations ------------------------------ */

BloomFilter *blf_create(unsigned int n, unsigned int m);

void blf_add(BloomFilter *filter, int x);

int blf_query(BloomFilter *filter, int x);

void blf_destroy(BloomFilter **filter);


#endif //HW_SYSPRO_BLOOMFILTER_H
