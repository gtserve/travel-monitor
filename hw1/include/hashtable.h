/*The Day Before hashtable.h
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

#include "util.h"

/* ---------------------------- Data Types ---------------------------------- */

typedef struct entry {
    struct entry *next;
    void *key;
    void *item;
    short key_size;
} EntryType;

typedef struct {
    EntryType **entries;
    size_t size;
} HashTable;

typedef struct {
    int index;
    EntryType *value;
    HashTable *table;
} HT_Iterator;

/* -------------------------- Basic Operations ------------------------------ */

HashTable *htb_create(size_t size);

void htb_insert(HashTable *hash_table, void *key, short key_size, void *item);

void *htb_search(HashTable *hash_table, void *key, short key_size);

// htb_remove

HT_Iterator *htb_iter_create(HashTable *table);

void *htb_iter_next(HT_Iterator *iterator);

EntryType *htb_iter_next_entry(HT_Iterator *iterator);

void htb_iter_destroy(HT_Iterator **iterator);

void htb_destroy(HashTable **hash_table, int free_items);

void htb_destroy_all(HashTable **hash_table, FP_item_free item_free);


#endif //SYSPRO_HW1_HASHTABLE_H
