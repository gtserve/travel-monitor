/* bloomfilter.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/bloomfilter.h"


/* -------------------------- Basic Operations ------------------------------ */

void blf_add(int element) {

    int k[BF_KEYS];
    for (int i = 0; i < BF_KEYS; ++i) {

    }

}


/* --------------------------- Hash Functions ------------------------------- */

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

unsigned long hash_i(unsigned int x, unsigned int i) {
    /* Only this function was changed from the original file h39jdk2.c to
     * function also as a wrapper and support unsigned integers. */

    return djb2((unsigned char *) &x) + i * sdbm((unsigned char *) &x) + i * i;
}
