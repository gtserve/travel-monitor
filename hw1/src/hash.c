/* hash.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include "../include/hash.h"


unsigned long hash_i(unsigned int i, unsigned char *key, short key_size) {
    return (djb2(key, key_size) + i * sdbm(key, key_size) + i * i);
}

unsigned long djb2(unsigned char *key, short key_size) {
    unsigned long hash = 5381;
    while (key_size) {
        hash = ((hash << 5) + hash) + (*(key++)); /* hash * 33 + c */
        key_size--;
    }
    return hash;
}

unsigned long sdbm(unsigned char *key, short key_size) {
    unsigned long hash = 0;
    while (key_size) {
        hash = (*key++) + (hash << 6) + (hash << 16) - hash;
        key_size--;
    }

    return hash;
}
