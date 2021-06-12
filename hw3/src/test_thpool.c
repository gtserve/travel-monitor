#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#include "../include/thpool.h"

#define NUM_THREADS 4
#define NUM_TASKS 8
#define TASK_CAP 3


void thread_func(void *sleep_t) {
    int sleep_time = *((int *) sleep_t);

    printf("Thread %d is about to sleep.\n", (int) pthread_self());
    sleep(sleep_time);
    printf("Thread %d just woke up!\n", (int) pthread_self());
    free(sleep_t);
}


int main(void) {

    ThreadPool *th_pool = thp_create(NUM_THREADS, TASK_CAP);

    for (int i = 0; i < NUM_TASKS; i++) {
        int *ip = (int *) malloc(sizeof(int));
        *ip = i + 1;
        thp_push_task(th_pool, thread_func, ip);
    }

    thp_destroy(&th_pool);

    return 0;
}

