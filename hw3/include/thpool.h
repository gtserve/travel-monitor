/* thpool.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW3_THPOOL_H
#define SYSPRO_HW3_THPOOL_H

#include <pthread.h>

#include "task_cqueue.h"


typedef struct {
    pthread_t *threads;
    int num_threads;
    int num_th_alive;
    int num_th_working;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    pthread_cond_t  idle_cond;
    TaskQueue *task_queue;
    int shutdown;
} ThreadPool;


/* Threads */
static void *thread_routine(void *arg);


/* ThreadPool */
ThreadPool *thp_create(int num_threads, int task_capacity);

void thp_destroy(ThreadPool **th_pool);

int thp_push_task(ThreadPool *th_pool, task_func_ptr function, void *arg);

void thp_block_until_done(ThreadPool *th_pool);


#endif //SYSPRO_HW3_THPOOL_H
