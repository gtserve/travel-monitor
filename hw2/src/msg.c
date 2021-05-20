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
#include <dirent.h>
#include <string.h>
#include "../include/msg.h"

#define SEP_TOKEN "$"

extern int BUFFER_SIZE;


void msg_send(char *message, int bytes, int pipe_fd) {

//    MessageHeader *msg_header = (MessageHeader *) malloc(sizeof(MessageHeader));

    MessageHeader msg_header;
//    memset(&msg_header, 0, sizeof(MessageHeader));

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

void send_directories(char *in_dir_name, int num_monitors, PipeChannel *pc) {

    DIR* in_dir;
    if (!(in_dir = opendir(in_dir_name))) {
        perror("Open Directory");
        exit(-1);
    }

    struct dirent *dent;


    int *sizes = (int *) calloc(num_monitors, sizeof(int));
    int i = 0;
    while ((dent = readdir(in_dir)) != NULL) {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        sizes[i % num_monitors] += (int) strlen(dent->d_name) + 1;
        i++;
    }

    char **dir_strs = (char **) calloc(num_monitors, sizeof(char *));
    for (int j = 0; j < num_monitors; j++)
        dir_strs[j] = (char *) calloc(sizes[j] + 1, sizeof(char));

    rewinddir(in_dir);

    i = 0;
    while ((dent = readdir(in_dir)) != NULL) {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        strcat(dir_strs[i % num_monitors], dent->d_name);
        strcat(dir_strs[i % num_monitors], SEP_TOKEN);
        i++;
    }

    for (int j = 0; j < num_monitors; j++) {
        msg_send(dir_strs[j], (int) strlen(dir_strs[j]) + 1, pc[j].writer_fd);
    }

    for (int j = 0; j < num_monitors; j++) {
        free(dir_strs[j]);
    }
    free(dir_strs);
    free(sizes);
}

int get_subdirectories(PipeChannel *pc, char ***dir_names, int monitor_id) {

    char *buffer;
    msg_get(&buffer, pc->reader_fd);

    printf("M%d: %s\n", monitor_id, buffer);

    int num_dirs = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == '$') {
            num_dirs++;
        }
    }

    printf("M%d: NUM_DIRS= %d\n", monitor_id, num_dirs);

    *dir_names = (char **) malloc(num_dirs * sizeof(char *));

    char *token = strtok(buffer, SEP_TOKEN);
    int offset = 0;
    for (int i = 0; token != NULL; i++) {
        printf("M%d: Token%d= %s\n", monitor_id, i, token);
        (*dir_names)[i] = (char *) malloc((int) strlen(token) + 1);
        offset += (int) strlen(token) + 1;
        printf("M%d: Buffer= %s, Offset= %d\n", monitor_id, buffer+offset, offset);
        strcpy((*dir_names)[i], token);
        printf("M%d: Dir%d= %s\n", monitor_id, i, (*dir_names)[i]);
        token = strtok(NULL, SEP_TOKEN);
    }

    printf("M%d: NUM_DIRS= %d\n", monitor_id, num_dirs);


    free(buffer);
    return num_dirs;
}
