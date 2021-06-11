/* socket.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <memory.h>

#include "../include/util.h"
#include "../include/sockets.h"

ServerData *sdt_create(int port) {
    ServerData *data = (ServerData *) malloc(sizeof(ServerData));

    data->server.sin_family = AF_INET;
    data->server.sin_addr.s_addr = htonl(INADDR_ANY);
    data->server.sin_port = htons(port);

    data->channel.port = port;
    data->server_ptr = (struct sockaddr *) &(data->server);
    data->client_ptr = (struct sockaddr *) &(data->client);
    data->client_len = sizeof(data->client);
    data->host = NULL;

    return data;
}

void sdt_destroy(ServerData **data) {
    close((*data)->channel.socket_fd);

    free((*data));

    (*data) = NULL;
}

ClientData *cdt_create(int num_channels) {
    ClientData *data = (ClientData *) malloc(sizeof(ClientData));

    data->channels = (SocketChannel *) malloc(sizeof(SocketChannel) * num_channels);
    data->num_channels = num_channels;
    data->servers = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in) * num_channels);
    data->server_ptrs = (struct sockaddr **) malloc(sizeof(struct sockaddr_in *) * num_channels);
    data->host = NULL;

    /* Get Host. */
    char host_name[NAME_SIZE];
    gethostname(host_name, NAME_SIZE);
    if ((data->host = gethostbyname(host_name)) == NULL) {
        herror("gethostbyname");
        exit(-1);
    }

    for (int i = 0; i < num_channels; i++) {
        data->servers[i].sin_family = AF_INET;
        memcpy(&(data->servers[i].sin_addr), data->host->h_addr, data->host->h_length);

        data->server_ptrs[i] = (struct sockaddr *) &(data->servers[i]);
    }

    /* Create Sockets. */
    for (int i = 0; i < num_channels; i++) {
        if ((data->channels[i].socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket creation");
            exit(-1);
        }
    }

    return data;
}

void cdt_destroy(ClientData **data) {
    for (int i = 0; i < (*data)->num_channels; i++)
        close((*data)->channels[i].socket_fd);

    free((*data)->channels);
    free((*data)->servers);
    free((*data)->server_ptrs);
    free((*data));

    (*data) = NULL;
}

void accept_connection(ServerData *s_data) {
    int temp_socket_fd;

    /* Create socket. */
    if ((temp_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("temp_socket creation");
        exit(-1);
    }

    int reuse = 1;
    if (setsockopt(temp_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror_exit("setsockopt");
    }

    /* Bind socket. */
    if (bind(temp_socket_fd, s_data->server_ptr, sizeof(s_data->server)) < 0) {
        close(temp_socket_fd);
        perror("bind");
        exit(-1);
    }

    /* Listen socket. */
    if (listen(temp_socket_fd, 5) < 0) {
        close(temp_socket_fd);
        perror("listen");
        exit(-1);
    }
    printf("[SR]: Listening for connections to port %d\n", s_data->channel.port);

    /* Accept */
    int new_socket_fd;
    if ((new_socket_fd = accept(temp_socket_fd, s_data->client_ptr, &(s_data->client_len))) < 0) {
        close(temp_socket_fd);
        perror("accept");
        exit(-1);
    }
    s_data->channel.socket_fd = new_socket_fd;

    if ((s_data->host = gethostbyaddr((char *) &(s_data->client.sin_addr.s_addr),
                                      sizeof(s_data->client.sin_addr.s_addr),
                                      s_data->client.sin_family)) == NULL) {
        perror("gethostbyaddr");
        exit(-1);
    }

    close(temp_socket_fd);
}

void establish_connection(ClientData *c_data) {

    /* Attempt connection with all channels. */
    for (int i = 0; i < c_data->num_channels; i++) {
        printf("[CL]: Attempting connection to %s, port=%d with SOCKET_FD=%d\n",
               c_data->host->h_name, c_data->channels[i].port, c_data->channels[i].socket_fd);

        if (connect(c_data->channels[i].socket_fd, c_data->server_ptrs[i],
                    sizeof(c_data->servers[i])) < 0) {
            close(c_data->channels[i].socket_fd);
            printf("[CL]: Connection to %s, port=%d with SOCKET_FD=%d FAILED!\n",
                   c_data->host->h_name, c_data->channels[i].port, c_data->channels[i].socket_fd);
        }

        printf("[CL]: Connecting to %s, port=%d with SOCKET_FD=%d\n",
               c_data->host->h_name, c_data->channels[i].port, c_data->channels[i].socket_fd);
    }
}
