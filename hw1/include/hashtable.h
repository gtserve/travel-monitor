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

#include <stddef.h>

/* ---------------------------- Data Types ---------------------------------- */

typedef struct entry {
    struct entry *next;
    void *key;
    void *item;
    short key_size;
//    short item_size;
} EntryType;

typedef struct {
    EntryType **entries;
    size_t size;
} HashTable;

/* -------------------------- Basic Operations ------------------------------ */

HashTable *htb_create(size_t size);

void htb_insert(HashTable *hash_table, void *key, short key_size, void *item);

void *htb_search(HashTable *hash_table, void *key, short key_size);

// TODO: htb_remove

void htb_destroy(HashTable **hash_table);


#endif //SYSPRO_HW1_HASHTABLE_H
