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

#include "../../include/bloomfilter.h"


int main(void) {

    int i = 17245;
    int j = 93754;

//    printf("%lu\n", hash_i((unsigned char*) &i, 0));
//    printf("%lu\n", hash_i((unsigned char*) &i, 1));
//    printf("%lu\n", hash_i((unsigned char*) &j, 0));
//    printf("%lu\n", hash_i((unsigned char*) &j, 1));

    for (int k = 0; k < 20; ++k) {
        printf("%lu\n", hash_i((unsigned char*) &i, k) % 10);
    }

    return 0;
}

