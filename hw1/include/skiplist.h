/* skip_list.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef HW_SYSPRO_SKIPLIST_H
#define HW_SYSPRO_SKIPLIST_H

#include "util.h"

/* ---------------------------- Data Types ---------------------------------- */

typedef struct sln {
    int key;
    void *item;
    struct sln **next;      // Array to next/right neighbour in each level.
} SLNode;

typedef struct {
    int nodes;              // Number of nodes.
    int levels;             // Number of levels.
    SLNode **heads;         // Array with the heads of each level/list.
    SLNode *tail;           // Bottom right-most node.
} SkipList;


/* -------------------------- Basic Operations ------------------------------ */

SkipList *skl_create();

void *skl_search(SkipList *list, int key);

SLNode * skl_get_next_node(SkipList *list, int key);

void skl_insert(SkipList *list, int key, void *item);

void skl_delete(SkipList *list, int key);

void skl_print(SkipList *list);

void skl_destroy(SkipList **list);

void skl_destroy_all(SkipList **list, FP_item_free item_free);


#endif //HW_SYSPRO_SKIPLIST_H
