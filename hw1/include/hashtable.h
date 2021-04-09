/* hashtable.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_HASHTABLE_H
#define SYSPRO_HW1_HASHTABLE_H

#define HT_NULL_KEY (-1)

typedef struct {
    char *key;
    char *item;
} TableEntry;

typedef struct {
    int key_size;
    int item_size;
} HashTable;



#endif //SYSPRO_HW1_HASHTABLE_H
