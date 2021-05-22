#include <stdio.h>
#include "../../include/util.h"

int main(void) {
    char *name = "../";
    printf("%d\n", count_files(name));
    return 0;
}

