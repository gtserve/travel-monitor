
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../../include/bitarray.h"
#include "../../include/hashtable.h"

#define N 1000
#define LIMIT 10000
#define Q 10

#define STR_BYTES(str) (strlen(str) + 1)


int main(void) {
    srand(time(NULL));
    
    HashTable *ht = htb_create(N/10);

    int x[N];
    int a = 0;
    for (int i = 0; i < N; i++) {
        x[i] = i;
        htb_insert(ht, &x[i], sizeof(int), &x[i]);
    }

    for (int i = 0; i < N; i++) {
        int *item = 0;
        item = htb_search(ht, &i, sizeof(int));
        if (item)
            printf("Item: %d\n", *item);
    }

    char *names[] = {"George", "Nikos", "Vaggelis", "A", "%&#%##$%@#", ""};
    char *age[] = {"23", "123", "666", "1", "99999999", "0"};

    for (int i = 0; i < 6; i++) {
        htb_insert(ht, names[i], STR_BYTES(names[i]), age[i]);
    }

    for (int i = 5; i >= 0; i--) {
        char *result = htb_search(ht, names[i], STR_BYTES(names[i]));
        printf("Name: %s, Age: %s, Res: %s\n", names[i], age[i], result);
    }

    htb_destroy(&ht, 0);

    return 0;
}
