/* hash.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_HASH_H
#define SYSPRO_HW1_HASH_H


unsigned long hash_i(unsigned int x, unsigned int i);

unsigned long djb2(unsigned char *str);

unsigned long sdbm(unsigned char *str);


#endif //SYSPRO_HW1_HASH_H
