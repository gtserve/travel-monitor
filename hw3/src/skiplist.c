/* skip_list.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164[@]di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/util.h"
#include "../include/skiplist.h"
#include "../include/data.h"

void trq_destroy(TravelRequest **travel_req);


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

void skl_destroy_nodes(SLNode *node) {
    /* Destroy all SLNodes recursively. */

    ERR_CHECK_NULL(node, "ERROR: [SL] destroy_nodes(): null node.\n");

    if (node->next[0] != NULL)
        skl_destroy_nodes(node->next[0]);

    // Destroy node.
    free(node->next);
    free(node);
}

void skl_destroy_nodes_all(SLNode *node, FP_item_free item_free) {
    /* Destroy all SLNodes recursively and their items. */

    ERR_CHECK_NULL(node, "ERROR: [SL] destroy_nodes_all(): null node.\n");
    ERR_CHECK_NULL(item_free, "ERROR: [SL] destroy_nodes_all(): null fun_ptr.\n");

    if (node->next[0] != NULL)
        skl_destroy_nodes_all(node->next[0], item_free);

    // Destroy node.
    free(node->next);
    item_free(&node->item);
    free(node);
}

void skl_destroy_nodes_req(SLNode *node) {
    /* Destroy all SLNodes recursively. */

    ERR_CHECK_NULL(node, "ERROR: [SL] destroy_nodes(): null node.\n");

    if (node->next[0] != NULL)
        skl_destroy_nodes_req(node->next[0]);

    // Destroy node.
    free(node->next);
    trq_destroy((TravelRequest **) &(node->item));
    free(node);
}

void skl_print_level(SLNode *node, int level) {
    /* Print a simple Linked List of nodes recursively. */

    ERR_CHECK_NULL(node, "ERROR: [SL] skl_print_level(): null node.\n");


    printf("[%d] -> ", node->key);
    if (node->next[level] != NULL) {
        skl_print_level(node->next[level], level);
    } else {
        printf("[NULL]\n");
    }
}


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

void *skl_search(SkipList *list, int key) {
    /* Search for an key in a Skip List. If key is found return its node or
     * else NULL. */

    ERR_CHECK_NULL_RETURN(list, "ERROR: [SL] skl_search(): null list.\n", NULL);


    // Fast checks to determine if key is one of the bounds or out of them.
    if (list->heads[0] != NULL) {
        if (key < list->heads[0]->key) {
            return NULL;
        } else if (key == list->heads[0]->key) {
            return list->heads[0]->item;
        }
    }
    if (list->tail != NULL) {
        if (key > list->tail->key) {
            return NULL;
        } else if (key == list->tail->key) {
            return list->tail->item;
        }
    }

    // Search the Skip List.
    if (list->nodes > 2) {
        SLNode **next = list->heads;
        for (int l = list->levels - 1; l >= 0; l--) {
            while ((next[l] != NULL) && (next[l]->key <= key)) {
                if (next[l]->key == key) {
                    return next[l]->item;
                }
                next = next[l]->next;
            }
        }
    }

    return NULL;
}

SLNode * skl_get_next_node(SkipList *list, int key) {
    /* Search for an key in a Skip List. If key is found return its node or
    * else NULL. */

    ERR_CHECK_NULL_RETURN(list, "ERROR: [SL] skl_search(): null list.\n", NULL);

    if (list->heads[0] == NULL)
        return NULL;

    if (key <= list->heads[0]->key)
        return list->heads[0];

    if (key > list->tail->key) {
        return NULL;
    } else if (key == list->tail->key) {
        return list->tail;
    }

    // Search the Skip List.
    SLNode **next = list->heads;
    for (int l = list->levels - 1; l >= 0; l--) {
        while ((next[l] != NULL) && (next[l]->key <= key)) {
            if (next[l]->key == key) {
                return next[l]->item;
            }
            next = next[l]->next;
        }
    }

    return next[0];
}

void skl_insert(SkipList *list, int key, void *item) {
    /* Insert a new key in the Skip List (duplicates not allowed). */

    ERR_CHECK_NULL(list, "ERROR: [SL] skl_insert(): null list.\n");


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
        while ((next[l] != NULL) && (next[l]->key <= key)) {
            if (next[l]->key == key) {
                // No duplicates allowed!
                printf("[SL] Key %d already inserted!\n", key);
                return;
            }
            current = next[l];
            next = current->next;
        }
        prev[l] = current;
    }

    // Create and initialize a new node.
    SLNode *new_node = (SLNode *) malloc(sizeof(SLNode));
    new_node->key = key;
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

    if (prev[0] == list->tail)
        list->tail = new_node;

    list->nodes++;
}

void skl_delete(SkipList *list, int key) {
    /* Delete an key from the Skip List. */

    ERR_CHECK_NULL(list, "ERROR: [SL] skl_delete(): null list.\n");


    // Find the target node and its left/previous neighbours.
    SLNode *prev[list->levels];
    SLNode *current = NULL;
    SLNode **next = list->heads;
    for (int l = list->levels - 1; l >= 0; l--) {
        while ((next[l] != NULL) && (next[l]->key < key)) {
            current = next[l];
            next = current->next;
        }
        prev[l] = current;
    }

    SLNode *target = NULL;
    if ((prev[0] == NULL) && (list->heads[0]->key == key)) {
        target = list->heads[0];
    } else if ((prev[0] != NULL) && (prev[0]->next[0]->key == key)) {
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

    printf("[SL] Couldn't find key %d for deletion!\n", key);
}

void skl_print(SkipList *list) {
    /* Print a whole Skip List. */

    ERR_CHECK_NULL(list, "ERROR: [SL] skl_print(): null list.\n");


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

void skl_destroy(SkipList **list) {
    /* Deallocate used space and destroy a Skip List. */

    ERR_CHECK_NULL(*list, "ERROR: [SL] skl_destroy(): null list.\n");

    // Destroy all nodes traversing recursively the lowest level.
    if ((*list)->heads[0] != NULL)
        skl_destroy_nodes((*list)->heads[0]);

    // Destroy Skip List.
    free((*list)->heads);
    free((*list));
    *list = NULL;
}

void skl_destroy_all(SkipList **list, FP_item_free item_free) {
    /* Deallocate used space and destroy a Skip List and its items. */

    ERR_CHECK_NULL(*list, "ERROR: [SL] skl_destroy_all(): null list.\n");
    ERR_CHECK_NULL(item_free, "ERROR: [SL] skl_destroy_all(): null fun_ptr.\n");

    // Destroy all nodes traversing recursively the lowest level.
    if ((*list)->heads[0] != NULL)
        skl_destroy_nodes_all((*list)->heads[0], item_free);

    // Destroy Skip List.
    free((*list)->heads);
    free((*list));
    *list = NULL;
}

void skl_destroy_req(SkipList **list) {
    /* Deallocate used space and destroy a Skip List. */

    ERR_CHECK_NULL(*list, "ERROR: [SL] skl_destroy(): null list.\n");

    // Destroy all nodes traversing recursively the lowest level.
    if ((*list)->heads[0] != NULL)
        skl_destroy_nodes_req((*list)->heads[0]);

    // Destroy Skip List.
    free((*list)->heads);
    free((*list));
    *list = NULL;
}