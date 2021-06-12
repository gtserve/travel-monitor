/* task_cqueue.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <malloc.h>

#include "../include/task_cqueue.h"
#include "../include/util.h"


TaskQueue *tsq_create(int capacity) {
    TaskQueue *queue = (TaskQueue *) malloc(sizeof(TaskQueue));

    pthread_mutex_init(&(queue->lock), NULL);

    queue->capacity = capacity;
    queue->size = 0;
    queue->head = 0;
    queue->tail = 0;

    queue->buffer = (Task **) malloc(sizeof(Task *) * capacity);

    return queue;
}

void tsq_destroy(TaskQueue **queue) {
    TaskQueue *que_ptr = (*queue);

    pthread_mutex_lock(&(que_ptr->lock));

    if (que_ptr->size != 0)
        perror("tsq_destroy: queue not empty");

    for (int i = que_ptr->head; que_ptr->size == 0; i = (i + 1) % que_ptr->capacity) {
        free(que_ptr->buffer[i]);
        que_ptr->size--;
    }
    free(que_ptr->buffer);

    pthread_mutex_unlock(&(que_ptr->lock));

    if (pthread_mutex_destroy(&(que_ptr->lock)) != 0)
        perror_exit("tsq_destroy: mutex lock destroy");

    free(que_ptr);
    (*queue) = NULL;
}

int tsq_push(TaskQueue *queue, Task *task) {
    pthread_mutex_lock(&(queue->lock));

    if (queue->size == queue->capacity) {
        pthread_mutex_unlock(&(queue->lock));
        return -1;
    }

    queue->buffer[queue->tail] = task;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;

    pthread_mutex_unlock(&(queue->lock));
    return 0;
}

Task *tsq_pop(TaskQueue *queue) {
    pthread_mutex_lock(&(queue->lock));

    if (queue->size == 0) {
        pthread_mutex_unlock(&(queue->lock));
        return NULL;
    }

    Task *task = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;

    if (queue->size == queue->capacity - 1) {
        /* Signal main thread to enter new task. */
        pthread_cond_signal(&(queue->has_space));
    }

    pthread_mutex_unlock(&(queue->lock));
    return task;
}
