#ifndef HW_SYSPRO_SKIP_LIST_H
#define HW_SYSPRO_SKIP_LIST_H


/* ---------------------------- Data Types ---------------------------------- */

typedef struct sln {
    int item;
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

SLNode *skl_search(SkipList *list, int item);

void skl_insert(SkipList *list, int item);

void skl_delete(SkipList *list, int item);

void skl_destroy(SkipList *list);


/* -------------------------- Auxiliary Functions --------------------------- */

int skl_flip_coin();

int skl_get_levels(int nodes);

void skl_destroy_level(SLNode *node);

void skl_print(SkipList *list);

void skl_print_level(SLNode *node, int level);


#endif //HW_SYSPRO_SKIP_LIST_H
