/* thpool.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include "stdlib.h"

#include "../include/thpool.h"
#include "../include/util.h"


static void *thread_routine(void *arg) {
    ThreadPool *th_pool = (ThreadPool *) arg;

    printf("Thread %d created.\n", (int) pthread_self());

    while (1) {
        pthread_mutex_lock(&(th_pool->lock));

        while ((th_pool->task_queue->size == 0) && (th_pool->shutdown == 0))
            pthread_cond_wait(&(th_pool->cond), &(th_pool->lock));

        if ((th_pool->shutdown == 1) && (th_pool->task_queue->size == 0)) {
            break;
        }

        th_pool->num_th_working++;

        /* Get task from queue. */
        Task *task = tsq_pop(th_pool->task_queue);

        pthread_mutex_unlock(&(th_pool->lock));

        /* Execute task */
        task->function(task->arg);
        free(task);

        /* Update working counter. */
        pthread_mutex_lock(&(th_pool->lock));
        th_pool->num_th_working--;
        pthread_mutex_unlock(&(th_pool->lock));
    }

    th_pool->num_th_alive--;

    printf("Thread %d is about to die.\n", (int) pthread_self());
    pthread_mutex_unlock(&(th_pool->lock));

    pthread_exit(NULL);
}


/* ThreadPool */
ThreadPool *thp_create(int num_threads, int task_capacity) {
    if (num_threads <= 0)
        return NULL;

    ThreadPool *th_pool = (ThreadPool *) malloc(sizeof(ThreadPool));

    th_pool->num_threads = num_threads;
    th_pool->num_th_alive = 0;
    th_pool->num_th_working = 0;
    th_pool->shutdown = 0;

    pthread_mutex_init(&(th_pool->lock), NULL);
    pthread_cond_init(&(th_pool->cond), NULL);

    th_pool->task_queue = tsq_create(task_capacity);

    th_pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&(th_pool->threads[i]), NULL, thread_routine, th_pool) != 0) {
            perror_exit("thp_create: pthread_create");
        }
        th_pool->num_th_alive++;
    }

    return th_pool;
}

void thp_destroy(ThreadPool **th_pool) {
    ThreadPool *th_pool_ptr = (*th_pool);

    pthread_mutex_lock(&(th_pool_ptr->lock));

    /* Mark thread pool for shut down. */
    th_pool_ptr->shutdown = 1;

    /* Wake up all threads so that they exit. */
    pthread_cond_broadcast(&(th_pool_ptr->cond));

    pthread_mutex_unlock(&(th_pool_ptr->lock));

    /* Wait for all threads to exit. */
    for (int i = 0; i < th_pool_ptr->num_threads; i++) {
        if (pthread_join(th_pool_ptr->threads[i], NULL) != 0) {
            perror_exit("thp_destroy: pthread_join");
        }
    }

    /* Destroy Data */
    free(th_pool_ptr->threads);
    pthread_mutex_destroy(&(th_pool_ptr->lock));
    pthread_cond_destroy(&(th_pool_ptr->cond));
    tsq_destroy(&(th_pool_ptr->task_queue));

    free(th_pool_ptr);
    (*th_pool) = NULL;
}

int thp_push_task(ThreadPool *th_pool, task_func_ptr function, void *arg) {
    pthread_mutex_lock(&(th_pool->lock));

    /* Create a new task. */
    Task *task = (Task *) malloc(sizeof(Task));
    task->function = function;
    task->arg = arg;

    /* Add task to queue. */
    while (tsq_push(th_pool->task_queue, task) != 0) {
        pthread_cond_wait(&(th_pool->task_queue->has_space), &(th_pool->lock));
    }

    /* Wake up a thread to execute task. */
    pthread_cond_signal(&(th_pool->cond));

    pthread_mutex_unlock(&(th_pool->lock));

    return 0;
}
