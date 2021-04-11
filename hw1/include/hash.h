/* hash.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_HASH_H
#define SYSPRO_HW1_HASH_H


unsigned long hash_i(unsigned int i, unsigned char *key, short key_size);

unsigned long djb2(unsigned char *key, short key_size);

unsigned long sdbm(unsigned char *key, short key_size);


#endif //SYSPRO_HW1_HASH_H
