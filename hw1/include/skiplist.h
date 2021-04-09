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

#include "types.h"

/* ---------------------------- Data Types ---------------------------------- */

typedef struct sln {
    int key;
    CitizenType *citizen;
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

CitizenType *skl_search(SkipList *list, int key);

void skl_insert(SkipList *list, CitizenType *item);

void skl_delete(SkipList *list, int key);

void skl_print(SkipList *list);

void skl_destroy(SkipList **list);


#endif //HW_SYSPRO_SKIPLIST_H
