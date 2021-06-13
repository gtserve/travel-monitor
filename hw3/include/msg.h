/* msg.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW2_MSG_H
#define SYSPRO_HW2_MSG_H


typedef struct {
    int num_packages;
    int payload_bytes;
} MessageHeader;


void msg_send(int fd, int pkg_size, char *payload, int payload_bytes);

int msg_get(int fd, int pkg_size, char **payload);

void safe_write(int fd, const void *buffer, size_t bytes);

void safe_read(int fd, void *buffer, size_t bytes);


#endif //SYSPRO_HW2_MSG_H
