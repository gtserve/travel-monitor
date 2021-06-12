/* task_queue.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>

#include "../include/task_queue.h"
#include "../include/util.h"


TaskQueue *tsq_create(int capacity) {
    TaskQueue *queue = (TaskQueue *) malloc(sizeof(TaskQueue));

    pthread_mutex_init(&(queue->lock), NULL);
    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void tsq_destroy(TaskQueue **queue) {
    TaskQueue *que_ptr = (*queue);

    pthread_mutex_lock(&(que_ptr->lock));

    Task *task = que_ptr->head;
    if (task != NULL)
        perror("tsq_destroy: queue not empty");

    while (task != NULL) {
        Task *temp = task;
        task = task->next;
        free(temp);
    }

    pthread_mutex_unlock(&(que_ptr->lock));

    if (pthread_mutex_destroy(&(que_ptr->lock)) != 0)
        perror_exit("tsq_destroy: mutex lock destroy");

    free(que_ptr);
    (*queue) = NULL;
}

void tsq_push(TaskQueue *queue, Task *task) {
    pthread_mutex_lock(&(queue->lock));

    if (queue->length == 0) {
        queue->head = task;
        queue->tail = task;
    } else {
        queue->tail->next = task;
        queue->tail = task;
    }
    queue->length++;

    pthread_mutex_unlock(&(queue->lock));
}

Task *tsq_pop(TaskQueue *queue) {
    pthread_mutex_lock(&(queue->lock));

    if (queue->length == 0)
        return NULL;

    Task *task = queue->head;
    if (queue->length == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }
    queue->length--;

    pthread_mutex_unlock(&(queue->lock));

    return task;
}