/* socket.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW3_SOCKETS_H
#define SYSPRO_HW3_SOCKETS_H

#define MY_PORT 9002

typedef struct {
    int *sockets;
    int size;
} SocketNetwork;

#endif //SYSPRO_HW3_SOCKETS_H
