/* comm.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW2_MSG_H
#define SYSPRO_HW2_MSG_H

#define PKG_SIZE 256

#define PIPE_PATH "/tmp"

typedef struct {
    int bytes;
    char message[PKG_SIZE];
} Package;


typedef struct {
    int num_packages;
    int msg_bytes;
} MessageHeader;


void msg_send(char *message, int bytes, int pipe_fd);

void msg_get(char **message, int pipe_fd);


#endif //SYSPRO_HW2_MSG_H
