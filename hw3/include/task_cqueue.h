/* task_cqueue.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/


#ifndef SYSPRO_HW3_TASK_CQUEUE_H
#define SYSPRO_HW3_TASK_CQUEUE_H

#include <pthread.h>

typedef void (*task_func_ptr)(void *arg);

typedef struct task {
    task_func_ptr function;
    void *arg;
} Task;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  has_space;
    int capacity;
    int size;
    int head;
    int tail;
    Task **buffer;
} TaskQueue;


TaskQueue *tsq_create(int capacity);

void tsq_destroy(TaskQueue **queue);

int tsq_push(TaskQueue *queue, Task *task);

Task *tsq_pop(TaskQueue *queue);


#endif //SYSPRO_HW3_TASK_CQUEUE_H
