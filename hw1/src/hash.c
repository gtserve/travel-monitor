/* hash.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include "../include/hash.h"


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
