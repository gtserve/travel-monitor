/* bloomfilter.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef HW_SYSPRO_BLOOMFILTER_H
#define HW_SYSPRO_BLOOMFILTER_H

#define BF_SIZE 100
#define BF_KEYS 2

/* ---------------------------- Data Types ---------------------------------- */



/* -------------------------- Basic Operations ------------------------------ */

void blf_add(int element);
void blf_query(int element);


/* --------------------------- Hash Functions ------------------------------- */

unsigned long djb2(unsigned char *str);
unsigned long sdbm(unsigned char *str);
unsigned long hash_i(unsigned int x, unsigned int i);


#endif //HW_SYSPRO_BLOOMFILTER_H
