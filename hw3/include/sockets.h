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

#include <netinet/in.h>

#define MY_PORT 9002

typedef struct {
    int socket_fd;
    int port;
} SocketChannel;

typedef struct {
    SocketChannel channel;
    struct sockaddr_in server;
    struct sockaddr_in client;
    struct sockaddr *server_ptr;
    struct sockaddr *client_ptr;
    socklen_t client_len;
    struct hostent *host;
} ServerData;

typedef struct {
    SocketChannel *channels;
    int num_channels;
    struct sockaddr_in *servers;
    struct sockaddr **server_ptrs;
    struct hostent *host;
} ClientData;


ServerData *sdt_create(int port);

void sdt_destroy(ServerData **data);

ClientData *cdt_create(int num_channels);

void cdt_destroy(ClientData **data);

void accept_connection(ServerData *s_data);

void establish_connection(ClientData *c_data);

#endif //SYSPRO_HW3_SOCKETS_H
