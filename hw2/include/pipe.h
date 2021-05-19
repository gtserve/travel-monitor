//
// Created by george on 19/5/21.
//

#ifndef SYSPRO_HW2_PIPE_H
#define SYSPRO_HW2_PIPE_H

#define PIPE_NAME_SIZE 30

typedef struct {
    int reader_fd;
    int writer_fd;
    char reader_name[PIPE_NAME_SIZE];
    char writer_name[PIPE_NAME_SIZE];
} PipeChannel;

#endif //SYSPRO_HW2_PIPE_H
