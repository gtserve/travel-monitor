/* task_queue.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW3_TASK_QUEUE_H
#define SYSPRO_HW3_TASK_QUEUE_H

#include <pthread.h>


typedef void (*task_func_ptr)(void *arg);

typedef struct task {
    task_func_ptr function;
    void *arg;
    struct task *next;
} Task;

typedef struct {
    pthread_mutex_t lock;
    int length;
    Task *head;
    Task *tail;
} TaskQueue;


TaskQueue *tsq_create(int capacity);

void tsq_destroy(TaskQueue **queue);

void tsq_push(TaskQueue *queue, Task *task);

Task *tsq_pop(TaskQueue *queue);


#endif //SYSPRO_HW3_TASK_QUEUE_H
