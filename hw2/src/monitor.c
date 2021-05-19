/* monitor.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

#include "../include/pipe.h"
#include "../include/msg.h"

int BUFFER_SIZE;

void get_subdirectories(PipeChannel *pc, char ***dirs);


int main(int argc, char *argv[]) {
    PipeChannel pc;
    int id = (int) strtol(argv[3], NULL, 10);

    strcpy(pc.reader_name, argv[2]);
    strcpy(pc.writer_name, argv[1]);

    printf("M%d: Received '%s' and '%s'\n", id, pc.reader_name, pc.writer_name);

    if ((pc.reader_fd = open(pc.reader_name, O_RDONLY)) == -1 ) {
        perror(pc.reader_name);
        exit(-1);
    }

    if ((pc.writer_fd = open(pc.writer_name, O_WRONLY)) == -1 ) {
        perror(pc.writer_name);
        exit(-1);
    }

    char **dirs = NULL;

//    printf("M%d: before dirs\n", id);

    get_subdirectories(&pc, &dirs);

//    struct pollfd pfds[1];
//    pfds[0].fd = pc.reader_fd;
//    pfds[0].events = POLLIN;
//
//    if (poll(pfds, 1, -1)) {
//        if (pfds[0].revents & POLLIN) {
//            char *buffer;
//            msg_get(&buffer, pc.reader_fd);
//            sprintf(buffer, "%s + M%s\n", buffer, argv[3]);
//            msg_send(buffer, 100, pc.writer_fd);
//            free(buffer);
//        }
//    }

    close(pc.reader_fd);
    close(pc.writer_fd);

    printf("M%d: DONE!\n", id);

    return 0;
}

void get_subdirectories(PipeChannel *pc, char ***dirs) {

    char *buffer;
    msg_get(&buffer, pc->reader_fd);

    printf("M: %s\n", buffer);

    free(buffer);
}
