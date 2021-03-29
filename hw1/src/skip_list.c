/* skip_list.c
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
#include <time.h>

#include "../include/skip_list.h"


/* -------------------------- Basic Operations ------------------------------ */

SkipList *skl_create() {
    /* Allocate required space and initialize a new Skip List. */

    SkipList *list = (SkipList *) malloc(sizeof(SkipList));
    list->nodes = 0;
    list->levels = 1;
    list->heads = (SLNode **) malloc(sizeof(SLNode *));
    list->heads[0] = NULL;
    list->tail = NULL;

    return list;
}

SLNode *skl_search(SkipList *list, int item) {
    /* Search for an item in a Skip List. If item is found return its node or
     * else NULL. */

    if (!list) {
        printf("[SL] Called skl_search for null list.\n");
        return NULL;
    }

    // Simple checks that can be done quickly.
    if (list->heads[0] != NULL) {
        if (item < list->heads[0]->item) {
            return NULL;
        } else if (item == list->heads[0]->item) {
            return list->heads[0];
        }
    }
    if (list->tail != NULL) {
        if (item > list->tail->item) {
            return NULL;
        } else if (item == list->heads[0]->item) {
            return list->heads[0];
        }
    }

    if (list->nodes > 2) {
        SLNode **next = list->heads;
        for (int l = list->levels - 1; l >= 0; l--) {
            while ((next[l] != NULL) && (next[l]->item <= item)) {
                if (next[l]->item == item) {
                    return next[l];
                }
                next = next[l]->next;
            }
        }
    }

    return NULL;
}

void skl_insert(SkipList *list, int item) {
    /* Insert a new item in the Skip List (duplicates not allowed). */

    if (!list) {
        printf("[SL] Called skl_insert for null list.\n");
        return;
    }

    // Determine the levels of the new node.
    int new_node_lvls = skl_get_levels(list->nodes);

    if (new_node_lvls > list->levels) {
        int old_list_lvls = list->levels;
        list->levels = new_node_lvls;
        list->heads = (SLNode **) realloc(list->heads,
                                          sizeof(SLNode *) * list->levels);
        for (int i = old_list_lvls; i < list->levels; ++i) {
            list->heads[i] = NULL;
        }
    }

    // Find the left/previous neighbours of the new node.
    SLNode *prev[list->levels];
    SLNode *current = NULL;
    SLNode **next = list->heads;
    for (int l = list->levels - 1; l >= 0; l--) {
        while ((next[l] != NULL) && (next[l]->item <= item)) {
            if (next[l]->item == item) {
                // No duplicates allowed!
                printf("[SL] Item %d is already inserted!\n", item);
                return;
            }
            current = next[l];
            next = current->next;
        }
        prev[l] = current;
    }

    // Create and initialize a new node.
    SLNode *new_node = (SLNode *) malloc(sizeof(SLNode));
    new_node->item = item;
    new_node->next = (SLNode **) malloc(sizeof(SLNode *) * new_node_lvls);
    new_node->next[0] = NULL;

    // Insert new node to Skip List.
    for (int l = 0; l < new_node_lvls; l++) {
        if (prev[l] == NULL) {
            new_node->next[l] = list->heads[l];
            list->heads[l] = new_node;
        } else {
            new_node->next[l] = prev[l]->next[l];
            prev[l]->next[l] = new_node;
        }
    }

    list->nodes++;
}

void skl_delete(SkipList *list, int item) {
    /* Delete an item from the Skip List. */

    if (!list) {
        printf("[SL] Called skl_delete for null list.\n");
        return;
    }

    // Find the target node and its left/previous neighbours.
    SLNode *prev[list->levels];
    SLNode *current = NULL;
    SLNode **next = list->heads;
    for (int l = list->levels - 1; l >= 0; l--) {
        while ((next[l] != NULL) && (next[l]->item < item)) {
            current = next[l];
            next = current->next;
        }
        prev[l] = current;
    }

    SLNode *target = NULL;
    if ((prev[0] == NULL) && (list->heads[0]->item == item)) {
        target = list->heads[0];
    } else if ((prev[0] != NULL) && (prev[0]->next[0]->item == item)) {
        target = prev[0]->next[0];
    }

    if (target != NULL) {
        for (int i = 0; i < list->levels; i++) {
            if ((prev[i] == NULL) && (list->heads[i] == target)) {
                list->heads[i] = target->next[i];
            } else if ((prev[i] != NULL) && (prev[i]->next[i] == target)) {
                prev[i]->next[i] = target->next[i];
            }
        }
        // Delete node.
        free(target->next);
        free(target);
        list->nodes--;
        return;
    }

    printf("[SL] Couldn't find item %d for deletion!\n", item);
}

void skl_destroy(SkipList *list) {
    /* Deallocate used space and destroy a Skip List. */

    if (!list) {
        printf("[SL] Called skl_destroy for null list.\n");
        return;
    }

    // Destroy all nodes using the lowest level.
    if (list->heads[0] != NULL)
        skl_destroy_level(list->heads[0]);

    // Destroy Skip List.
    free(list->heads);
    free(list);
}


/* -------------------------- Auxiliary Functions --------------------------- */

int skl_flip_coin() {
    return (rand() % 2);
}

int skl_get_levels(int nodes) {
    int num_levels = 1;                         // Minimum
    int max = (int) log2(nodes) + 1;
    while ((num_levels < max) && (skl_flip_coin() == 1)) {
        num_levels++;
    }
    return num_levels;
}

void skl_destroy_level(SLNode *node) {
    /* Destroy a simple Linked List of nodes recursively. */

    if (!node) {
        printf("[SL] Called skl_destroy_level for null node.\n");
        return;
    }

    if (node->next[0] != NULL)
        skl_destroy_level(node->next[0]);

    // Destroy node.
    free(node->next);
    free(node);
}


void skl_print(SkipList *list) {
    /* Print a whole Skip List. */

    if (!list) {
        printf("[SL] Called skl_print for null list.\n");
        return;
    }

    printf("Skip List (N%d, L%d):\n", list->nodes, list->levels);
    for (int l = list->levels - 1; l >= 0; l--) {
        printf("L%d: ", l);
        if (list->heads[l] != NULL) {
            skl_print_level(list->heads[l], l);
        } else {
            printf("NULL\n");
        }
    }
}

void skl_print_level(SLNode *node, int level) {
    /* Print a simple Linked List of nodes recursively. */

    if (!node) {
        printf("[SL] Called skl_print_level for null node.\n");
        return;
    }

    printf("[%d] -> ", node->item);
    if (node->next[level] != NULL) {
        skl_print_level(node->next[level], level);
    } else {
        printf("[NULL]\n");
    }
}
