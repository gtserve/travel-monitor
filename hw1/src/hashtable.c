/* hashtable.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../include/hash.h"
#include "../include/hashtable.h"

#define NULL_KEY (-1)
#define EQUAL_KEYS(k1, k1s, k2, k2s)\
(((k1s) == (k2s)) && (memcmp((k1), (k2), (k1s)) == 0))


/* ------------------------- Auxiliary Functions ---------------------------- */


unsigned long hash_key(void *key, short key_size) {
    return hash_i(1, (unsigned char *) key, key_size);
}


/* -------------------------- Basic Operations ------------------------------ */

HashTable *htb_create(size_t size) {

    int primes[6] = {1, 11, 101, 1009, 10009, 1000003};
    int rank = (int) round(log10((double) size));
    if (rank < 0)
        rank = 0;
    if (rank > 5)
        rank = 5;
    size = primes[rank];

    HashTable *hash_table = (HashTable *) malloc(sizeof(HashTable));
    hash_table->entries = (EntryType **) calloc(size, sizeof(EntryType *));
    hash_table->size = size;

    return hash_table;
}

void htb_insert(HashTable *hash_table, void *key, short key_size, void *item) {
    unsigned long index = hash_key(key, key_size) % hash_table->size;

    // Create new entry.
    EntryType *entry = (EntryType *) malloc(sizeof(EntryType));
    entry->next = NULL;

    entry->key = malloc(key_size);
    memcpy(entry->key, key, key_size);
    entry->key_size = key_size;

    entry->item = item;

    EntryType *head = hash_table->entries[index];
    if (head)
        entry->next = head;
    hash_table->entries[index] = entry;
}

void *htb_search(HashTable *hash_table, void *key, short key_size) {
    unsigned long index = hash_key(key, key_size) % hash_table->size;

    EntryType *cur_entry = hash_table->entries[index];
    while ((cur_entry) &&
            (!EQUAL_KEYS(key, key_size, cur_entry->key, cur_entry->key_size))) {
        cur_entry = cur_entry->next;
    }

    return ((cur_entry) ? cur_entry->item : NULL);
}

void htb_destroy(HashTable **hash_table, int free_items) {

    for (int i = 0; i < (*hash_table)->size; i++) {
        EntryType *current = (*hash_table)->entries[i];
        while (current) {
            EntryType *next = current->next;
            free(current->key);
            if (free_items)
                free(current->item);
            free(current);
            current = next;
        }
    }
    free((*hash_table)->entries);
    free(*hash_table);
    *hash_table = NULL;
}

void htb_destroy_all(HashTable **hash_table, FP_item_free item_free) {
    for (int i = 0; i < (*hash_table)->size; i++) {
        EntryType *current = (*hash_table)->entries[i];
        while (current) {
            EntryType *next = current->next;
            free(current->key);
            (*item_free)(&current->item);
            free(current);
            current = next;
        }
    }
    free((*hash_table)->entries);
    free(*hash_table);
    *hash_table = NULL;
}


HT_Iterator *htb_iter_create(HashTable *table) {
    HT_Iterator *iter = (HT_Iterator *) malloc(sizeof(HT_Iterator));
    iter->index = -1;
    iter->value = NULL;
    iter->table = table;
    return iter;
}

void *htb_iter_next(HT_Iterator *iterator) {
    if ((iterator->index != -1) && (iterator->value == NULL))
        return NULL;

    if (iterator->index == -1) {
        iterator->index = 0;
        iterator->value = iterator->table->entries[0];
    } else {
        iterator->value = iterator->value->next;
    }

    while (iterator->value == NULL) {
        if (++(iterator->index) == iterator->table->size) {
            return NULL;
        }
        iterator->value = iterator->table->entries[iterator->index];
    }

    return iterator->value->item;
}

EntryType *htb_iter_next_entry(HT_Iterator *iterator) {
    if ((iterator->index != -1) && (iterator->value == NULL))
        return NULL;

    if (iterator->index == -1) {
        iterator->index = 0;
        iterator->value = iterator->table->entries[0];
    } else {
        iterator->value = iterator->value->next;
    }

    while (iterator->value == NULL) {
        if (++(iterator->index) == iterator->table->size) {
            return NULL;
        }
        iterator->value = iterator->table->entries[iterator->index];
    }

    return iterator->value;
}

void htb_iter_destroy(HT_Iterator **iterator) {
    free(*iterator);
    *iterator = NULL;
}