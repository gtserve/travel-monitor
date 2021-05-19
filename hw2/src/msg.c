/* comm.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include "../include/msg.h"

extern int BUFFER_SIZE;


void msg_send(char *message, int bytes, int pipe_fd) {

//    MessageHeader *msg_header = (MessageHeader *) malloc(sizeof(MessageHeader));

    MessageHeader msg_header;
    memset(&msg_header, 0, sizeof(MessageHeader));

    msg_header.msg_bytes = bytes;
    msg_header.num_packages = 1;

    if (write(pipe_fd, &msg_header, sizeof(MessageHeader)) == -1) {
        perror("msg_send header");
        exit(-1);
    }

    if (write(pipe_fd, message, bytes) == -1 ) {
        perror("msg_send");
        exit(-1);
    }

//    printf("msg_send: msg = %s\n", message);
//    free(msg_header);
}

void msg_get(char **message, int pipe_fd) {
//    MessageHeader *msg_header = (MessageHeader *) malloc(sizeof(MessageHeader));
    MessageHeader msg_header;


    if (read(pipe_fd, &msg_header, sizeof(MessageHeader)) == -1) {
        perror("msg_get header");
        exit(-1);
    }

//    printf("MSG.bytes = %d\n", msg_header.msg_bytes);
//    printf("MSG.num_packages = %d\n", msg_header.num_packages);

    *message = (char *) malloc(msg_header.msg_bytes);

    if (read(pipe_fd, *message, msg_header.msg_bytes) == -1) {
        perror("msg_get");
        exit(-1);
    }

//    printf("msg_get: msg = %s\n", *message);
//    free(msg_header);
}