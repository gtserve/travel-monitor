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


/* ------------------------- Auxiliary Functions ---------------------------- */


unsigned long hash_key(void *key, short key_size) {
    return hash_i(1, (unsigned char *) key, key_size);
}

int equal_keys(void *key1, short key1_size, void *key2, short key2_size) {
    return ((key1_size == key2_size) && (memcmp(key1, key2, key1_size) == 0));
}


/* -------------------------- Basic Operations ------------------------------ */

HashTable *htb_create(size_t size) {

    int primes[5] = {11, 101, 1009, 10009, 1000003};
    int rank = (int) round(log10((double) size)) - 1;
    if (rank < 0)
        rank = 0;
    if (rank > 4)
        rank = 4;
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
            (!equal_keys(key, key_size, cur_entry->key, cur_entry->key_size))) {
        cur_entry = cur_entry->next;
    }

    return ((cur_entry) ? cur_entry->item : NULL);
}


void htb_destroy(HashTable **hash_table) {

    for (int i = 0; i < (*hash_table)->size; i++) {
        EntryType *current = (*hash_table)->entries[i];
        while (current) {
            EntryType *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free((*hash_table)->entries);
    free(*hash_table);
    *hash_table = NULL;
}