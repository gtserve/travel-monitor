/* travelmonitor.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <poll.h>
#include <fcntl.h>

#include "../include/util.h"
#include "../include/msg.h"
#include "../include/pipe.h"

#define PROG_NAME   "travelMonitor"
#define USAGE_STR   "Program Usage:\n %s [-m numMonitors] [-b bufferSize] [-s sizeOfBloom] " \
                    "[-i input_dir]\n"

#define PATH_SIZE 255
#define PIPE_PERMS 0666
#define MNTR_ID_SIZE 20


typedef struct {
    int num_monitors;
    int buffer_size;
    int bloom_size;
    char *input_dir;
} ProgArguments;

void usage(char *prog_name);

int BUFFER_SIZE;


int main(int argc, char **argv) {

    ProgArguments p_args = {0, 0, 0, NULL};
    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, "m:b:s:i:")) != EOF) {
        // Options: [-m numMonitors] [-b bufferSize] [-s sizeOfBloom] [-i input_dir]
        switch (c) {
            case 'm': {
                ERR_CHECK_ARG((!str_is_int(optarg)), ERR_MSG_NOT_INT, "numMonitors");
                p_args.num_monitors = (int) strtol(optarg, NULL, 10);
                ERR_CHECK_ARG((p_args.num_monitors < 0), ERR_MSG_NOT_POS, "numMonitors");
                break;
            }
            case 'b': {
                ERR_CHECK_ARG((!str_is_int(optarg)), ERR_MSG_NOT_INT, "bufferSize");
                p_args.buffer_size = (int) strtol(optarg, NULL, 10);
                ERR_CHECK_ARG((p_args.buffer_size < 0), ERR_MSG_NOT_POS, "bufferSize");
                break;
            }
            case 's': {
                ERR_CHECK_ARG((!str_is_int(optarg)), ERR_MSG_NOT_INT, "sizeofBloom");
                p_args.bloom_size = (int) strtol(optarg, NULL, 10);
                ERR_CHECK_ARG((p_args.bloom_size < 0), ERR_MSG_NOT_POS, "sizeofBloom");
                break;
            }
            case 'i': {
                DIR *in_dir = opendir(optarg);
                ERR_CHECK_ARG((in_dir == NULL), ERR_MSG_DIR_NEXST, optarg);
                p_args.input_dir = optarg;
                break;
            }
            case '?': {
                if ((optopt == 'm') || (optopt == 'b') || (optopt == 's') || (optopt == 'i')) {
                    fprintf(stderr, ERR_MSG_ARG, optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, ERR_MSG_UNKNOWN, optopt);
                } else {
                    fprintf(stderr, ERR_MSG_NOT_PRINT, optopt);
                }
                usage(argv[0]);
                break;
            }
            default:
                fprintf(stderr, "UNKNOWN ERROR!");
                exit(EXIT_FAILURE);
        }
    }

    printf("NUM_MONITORS:  %d\n", p_args.num_monitors);
    printf("BUFFER_SIZE:   %d\n", p_args.buffer_size);
    printf("SIZE_OF_BLOOM: %d\n", p_args.bloom_size);
    printf("INPUT_DIR:     %s\n", p_args.input_dir);


    BUFFER_SIZE = p_args.buffer_size;

    char m_path[PATH_SIZE];
    if (getcwd(m_path, PATH_SIZE) == NULL) {
        perror("getcwd");
        exit(-1);
    }
    strcpy(m_path + strlen(m_path), "/monitor");
//    printf("[INFO] Monitor path: '%s'\n", m_path);

    // Create Monitor arguments
    char *m_args[5];
    m_args[0] = (char *) malloc(strlen("monitor") + 1);
    strcpy(m_args[0], "monitor");
    m_args[1] = (char *) malloc(PIPE_NAME_SIZE);    // pipe-reader name
    m_args[2] = (char *) malloc(PIPE_NAME_SIZE);    // pipe-writer name
    m_args[3] = (char *) malloc(MNTR_ID_SIZE);      // monitor id
    m_args[4] = NULL;

    PipeChannel pipe_channels[p_args.num_monitors];
    struct pollfd pfds[p_args.num_monitors];

    for (int i = 0; i < p_args.num_monitors; i++) {

        // Create Named Pipe (Reader)
        sprintf(pipe_channels[i].reader_name, "%s/my_pipe_M%d_R", PIPE_PATH, i);
        if (mkfifo(pipe_channels[i].reader_name, PIPE_PERMS) == -1) {
            perror(pipe_channels[i].reader_name);
            exit(-1);
        }
        strcpy(m_args[1], pipe_channels[i].reader_name);

        // Create Named Pipe (Writer)
        sprintf(pipe_channels[i].writer_name, "%s/my_pipe_M%d_W", PIPE_PATH, i);
        if (mkfifo(pipe_channels[i].writer_name, PIPE_PERMS) == -1) {
            perror(pipe_channels[i].writer_name);
            exit(-1);
        }
        strcpy(m_args[2], pipe_channels[i].writer_name);

        // Monitor ID
        sprintf(m_args[3], "%d", i);

        pid_t pid = fork();
        if (-1 == pid) {
            perror("fork");
            exit(-1);
        }

        if (pid == 0) {
            if (execvp(m_path, m_args) < 0) {
                perror("exec");
                exit(-1);
            }
        }
    }

    for (int i = 0; i < p_args.num_monitors; i++) {
        if ((pipe_channels[i].writer_fd = open(pipe_channels[i].writer_name, O_WRONLY)) == -1 ) {
            perror(pipe_channels[i].writer_name);
            exit(-1);
        }

        if ((pipe_channels[i].reader_fd = open(pipe_channels[i].reader_name, O_RDONLY)) == -1 ) {
            perror(pipe_channels[i].reader_name);
            exit(-1);
        }

        pfds[i].fd = pipe_channels[i].reader_fd;
        pfds[i].events = POLLIN;
    }

    send_directories(p_args.input_dir, p_args.num_monitors, pipe_channels);

//    char **dir_names = NULL;
//
//    get_subdirectories(pipe_channels[0], &dir_names, 0);

//    sleep(5);

//    for (int i = 0; i < p_args.num_monitors; i++) {
//        char buffer[100] = "TM";
//        msg_send(buffer, 100, pipe_channels[i].writer_fd);
//    }
//
//    int count = 0;
//    while (poll(pfds, p_args.num_monitors, -1)) {
//        for (int i = 0; i < p_args.num_monitors; i++) {
//            if (pfds[i].revents & POLLIN) {
//                char *buffer;
//                msg_get(&buffer, pipe_channels[i].reader_fd);
//                printf("TM: Received: %s\n", buffer);
//                free(buffer);
//                count++;
//            }
//        }
//        if (count == p_args.num_monitors) {
//            break;
//        }
//    }

    for (int i = 0; i < p_args.num_monitors; i++) {
        close(pipe_channels[i].reader_fd);
        close(pipe_channels[i].writer_fd);
    }

    for (int i = 0; i < p_args.num_monitors; i++) {
        if (unlink(pipe_channels[i].reader_name) == -1) {
            perror(pipe_channels[i].reader_name);
            exit(-1);
        }

        if (unlink(pipe_channels[i].writer_name) == -1) {
            perror(pipe_channels[i].writer_name);
            exit(-1);
        }
    }

    printf("TM: Done\n");

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}
