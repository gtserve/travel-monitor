/* pipe.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW2_PIPE_H
#define SYSPRO_HW2_PIPE_H

#define PIPE_NAME_SIZE 32

typedef struct {
    int reader_fd;
    int writer_fd;
    char reader_name[PIPE_NAME_SIZE];
    char writer_name[PIPE_NAME_SIZE];
} PipeChannel;

#endif //SYSPRO_HW2_PIPE_H
