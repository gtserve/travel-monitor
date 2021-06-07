/* msg.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#include "../include/msg.h"


void msg_send(int pipe_fd, int pkg_size, char *payload, int payload_bytes) {

    // Create MessageHeader.
    MessageHeader msg_header;
    msg_header.payload_bytes = payload_bytes;
    msg_header.num_packages = (int) ceil(payload_bytes / (double) pkg_size);

    // Send MessageHeader.
    safe_write(pipe_fd, &msg_header, sizeof(MessageHeader));

    // Send payload.
    int offset = 0;
    for (int i = 0; i < msg_header.num_packages - 1; i++) {
        safe_write(pipe_fd, payload + offset, pkg_size);
        offset += pkg_size;
    }
    // Last write() may write less bytes than package size.
    safe_write(pipe_fd, payload + offset, payload_bytes - offset);
}

int msg_get(int pipe_fd, int pkg_size, char **payload) {

    // Get MessageHeader.
    MessageHeader msg_header;
    safe_read(pipe_fd, &msg_header, sizeof(MessageHeader));

    // Create payload.
    *payload = (char *) malloc(msg_header.payload_bytes);

    // Get payload.
    int offset = 0;
    for (int i = 0; i < msg_header.num_packages - 1; i++) {
        safe_read(pipe_fd, (*payload) + offset, pkg_size);
        offset += pkg_size;
    }
    // Last read() may read less bytes than package size.
    safe_read(pipe_fd, (*payload) + offset, msg_header.payload_bytes - offset);

    return msg_header.payload_bytes;
}

void safe_write(int fd, const void *buffer, size_t bytes) {
    size_t bytes_w = 0;

    while (bytes_w != bytes) {
        size_t cur_bytes;
        if ((cur_bytes = write(fd, buffer + bytes_w, bytes - bytes_w)) < 0) {
            perror("[safe_write]");
            exit(-1);
        }
        bytes_w += cur_bytes;
    }
}

void safe_read(int fd, void *buffer, size_t bytes) {
    size_t bytes_r = 0;

    while (bytes_r != bytes) {
        size_t cur_bytes;
        if ((cur_bytes = read(fd, buffer + bytes_r, bytes - bytes_r)) < 0) {
            perror("[safe_read]");
            exit(-1);
        }
        bytes_r += cur_bytes;
    }
}
