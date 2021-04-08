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
