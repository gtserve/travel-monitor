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
#include <math.h>
#include <wait.h>

#include "../include/util.h"
#include "../include/msg.h"
#include "../include/pipe.h"
#include "../include/data.h"
#include "../include/comm_protocol.h"

#define PROG_NAME   "travelMonitor"
#define USAGE_STR   "Program Usage:\n %s [-m numMonitors] [-b bufferSize] [-s sizeOfBloom] " \
                    "[-i input_dir]\n"


#define PIPE_PERMS 0666
#define INT_STR_SIZE 20


typedef struct {
    int num_monitors;
    int buffer_size;
    int bloom_size;
    char input_dir[PATH_SIZE];
} ProgArguments;

void usage(char *prog_name);

void send_directories(char *in_dir_name, int num_monitors, PipeChannel *pc, TM_Data **tm_data);

void get_filters(TM_Data **mon_data, int num_monitors, struct pollfd *pfds);

void get_filter(TM_Data *mon_data, int pipe_fd);


int main(int argc, char **argv) {

    ProgArguments p_args = {0, 0, 0};
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
                closedir(in_dir);
                strcpy(p_args.input_dir, optarg);
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

    printf("[TM]: Arguments=\n");
    printf("  NUM_MONITORS:  %d\n", p_args.num_monitors);
    printf("  BUFFER_SIZE:   %d\n", p_args.buffer_size);
    printf("  SIZE_OF_BLOOM: %d\n", p_args.bloom_size);
    printf("  INPUT_DIR:     %s\n", p_args.input_dir);

    char m_path[PATH_SIZE];
    if (getcwd(m_path, PATH_SIZE) == NULL) {
        perror("getcwd");
        exit(-1);
    }
    strcpy(m_path + strlen(m_path), "/monitor");
//    printf("[INFO] Monitor path: '%s'\n", m_path);

    // Create Monitor arguments
    int num_m_args = 8;
    char *m_args[num_m_args];

    m_args[0] = (char *) malloc(strlen("monitor") + 1);
    strcpy(m_args[0], "monitor");                           // exe name

    m_args[1] = (char *) malloc(INT_STR_SIZE);                        // monitor id

    m_args[2] = (char *) malloc(INT_STR_SIZE);
    sprintf(m_args[2], "%d", p_args.buffer_size);           // buffer size

    m_args[3] = (char *) malloc(50);
    sprintf(m_args[3], "%d", p_args.bloom_size);            // bloom filter size

    m_args[4] = (char *) malloc(strlen(p_args.input_dir) + 1);
    strcpy(m_args[4], p_args.input_dir);                         // input_dir path

    m_args[5] = (char *) malloc(PIPE_NAME_SIZE);                      // pipe-reader name
    m_args[6] = (char *) malloc(PIPE_NAME_SIZE);                      // pipe-writer name

    m_args[num_m_args - 1] = NULL;


    PipeChannel pipe_channels[p_args.num_monitors];
    struct pollfd pfds[p_args.num_monitors];

    // Save monitors process ids
    pid_t *monitor_pids = (pid_t *) malloc(sizeof(pid_t) * p_args.num_monitors);

    for (int i = 0; i < p_args.num_monitors; i++) {

        // Create Named Pipe (Reader)
        sprintf(pipe_channels[i].reader_name, "%s/my_pipe_M%d_R", PIPE_PATH, i);
        if (mkfifo(pipe_channels[i].reader_name, PIPE_PERMS) == -1) {
            perror(pipe_channels[i].reader_name);
            exit(-1);
        }
        strcpy(m_args[5], pipe_channels[i].reader_name);

        // Create Named Pipe (Writer)
        sprintf(pipe_channels[i].writer_name, "%s/my_pipe_M%d_W", PIPE_PATH, i);
        if (mkfifo(pipe_channels[i].writer_name, PIPE_PERMS) == -1) {
            perror(pipe_channels[i].writer_name);
            exit(-1);
        }
        strcpy(m_args[6], pipe_channels[i].writer_name);

        // Monitor ID
        sprintf(m_args[1], "%d", i);

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
        } else {
            monitor_pids[i] = pid;
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

    // Create Data for every Monitor
    TM_Data **tm_data = (TM_Data **) malloc(sizeof(TM_Data *) * p_args.num_monitors);
    for (int i = 0; i < p_args.num_monitors; i++)
        tm_data[i] = mtr_create(p_args.bloom_size, 10000);

    // Send Directories
    DIR* in_dir;
    if (!(in_dir = opendir(p_args.input_dir))) {
        perror(p_args.input_dir);
        exit(-1);
    }
    struct dirent *dent;
    for (int i = 0; ((dent = readdir(in_dir)) != NULL); i++) {
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        // Send directory name to Monitor
        char *message = NULL;
        int msg_bytes = encode_str(CDIR, dent->d_name, &message);
        msg_send(pipe_channels[i % p_args.num_monitors].writer_fd,
                 p_args.buffer_size, message, msg_bytes);

        // Create new country
        CountryType *country = (CountryType *) malloc(sizeof(CountryType));
        STR_CPY(dent->d_name, country->name);
        country->population = 0;
        // Save country
        htb_insert(tm_data[i % p_args.num_monitors]->countries, dent->d_name,
                   STR_BYTES(dent->d_name), country);
    }
    closedir(in_dir);

    // Send Directories is done.
    for (int i = 0; i < p_args.num_monitors; i++) {
        char *message = NULL;
        char *buffer = "DONE";
        int msg_bytes = encode_str(CDIRS_DONE, buffer, &message);
        msg_send(pipe_channels[i].writer_fd, p_args.buffer_size, message, msg_bytes);
    }

    // Wait for children
    for (int i = 0; i < p_args.num_monitors; i++)
        wait(&(monitor_pids[i]));

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

    for (int i = 0; i < num_m_args; i++)
        free(m_args[i]);

    for (int i = 0; i < p_args.num_monitors; i++)
        mtr_destroy(&(tm_data[i]));

    free(monitor_pids);

    printf("[TM]: DONE!\n");

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}

//void send_directories(char *in_dir_name, int num_monitors, PipeChannel *pc,
//                      TM_Data **tm_data) {
//
//    DIR* in_dir;
//    if (!(in_dir = opendir(in_dir_name))) {
//        perror(in_dir_name);
//        exit(-1);
//    }
//
//    struct dirent *dent;
//    int i = 0;
//    while ((dent = readdir(in_dir)) != NULL) {
//        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
//            continue;
//
//        char *message = NULL;
//        encode_str(CDIR, dent->d_name, &message);
//        msg_send(pc[i % num_monitors].writer_fd, )
//
//        // Create new country
//        CountryType *country = (CountryType *) malloc(sizeof(CountryType));
//        STR_CPY(dent->d_name, country->name);
//        country->population = 0;
//        // Save country
//        htb_insert(tm_data[i % num_monitors]->countries, dent->d_name,
//                   STR_BYTES(dent->d_name), country);
//        i++;
//    }
//
//    char **dir_strs = (char **) calloc(num_monitors, sizeof(char *));
//    for (int j = 0; j < num_monitors; j++)
//        dir_strs[j] = (char *) calloc(sizes[j] + 1, sizeof(char));
//
//    rewinddir(in_dir);
//
//    i = 0;
//    while ((dent = readdir(in_dir)) != NULL) {
//        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
//            continue;
//
//        strcat(dir_strs[i % num_monitors], dent->d_name);
//        strcat(dir_strs[i % num_monitors], SEP_TOKEN);
//        i++;
//    }
//
//    for (int j = 0; j < num_monitors; j++) {
//        msg_send(pc[j].writer_fd, 0, dir_strs[j], (int) strlen(dir_strs[j]) + 1);
//    }
//
//    for (int j = 0; j < num_monitors; j++) {
//        free(dir_strs[j]);
//    }
//    free(dir_strs);
//    free(sizes);
//    closedir(in_dir);
//}
//
//void get_filters(TM_Data **mon_data, int num_monitors, struct pollfd *pfds) {
//    int count = 0;
//    while (poll(pfds, num_monitors, -1)) {
//        for (int i = 0; i < num_monitors; i++) {
//            if (pfds[i].revents & POLLIN) {
//                get_filter(mon_data[i], pfds[i].fd);
//                count++;
//            }
//        }
//        if (count == num_monitors) {
//            return;
//        }
//    }
//}

//void get_filter(TM_Data *mon_data, int pipe_fd) {
//
//    int bf_array_size = (int) ceil(mon_data->bloom_size / (double) BAR_TYPE_BITS);
//    int bf_bytes = (int) (bf_array_size * sizeof(BAR_TYPE));
//
//    char *filter_msg = NULL;
//    int msg_size = msg_get(pipe_fd, 0, &filter_msg);
//    printf("%.*s\n", msg_size-1, filter_msg);
//
//    int offset = 0;
//    VirusInfo *virus = NULL;
//    char *token = strtok(filter_msg, SEP_TOKEN);
//    while (token != NULL) {
//
//        // Virus name length
//        int virus_name_len = (int) strtol(token, NULL, 10);
//        token = strtok(NULL, SEP_TOKEN);
//
//        printf("[TM]: Virus_Name_Len= %d\n", virus_name_len);
//
//        // Virus name
//        char virus_name[virus_name_len + 1];
//        strcpy(virus_name, token);
//        token = strtok(NULL, SEP_TOKEN);
//
//        printf("[TM]: Virus_Name= %s\n", virus_name);
//
//
//
//        memcpy(&virus_name_len, filter_msg + offset, sizeof(int));
//        offset += sizeof(int);
//        printf("%d|", (int) virus_name_len);
//
//        char *virus_name = (char *) malloc(virus_name_len + 1);
//        memcpy(virus_name, filter_msg + offset, virus_name_len);
//        virus_name[virus_name_len] = '\0';
//        offset += (int) virus_name_len;
//        printf("%s|", virus_name);
//        fflush(stdin);
//
//        // Create new virus
//        virus = vir_create(virus_name, mon_data->bloom_size, 10000);
//        htb_insert(mon_data->viruses, virus_name,STR_BYTES(virus_name), virus);
//
//        // Virus bloom filter array
//        BAR_TYPE *bf_new_array = (BAR_TYPE *) calloc(bf_array_size, sizeof(BAR_TYPE));
//        memcpy(bf_new_array, filter_msg + offset, bf_bytes);
//        offset += bf_bytes;
//        printf("%d||", bf_bytes);
//
//        // Destroy old array
//        free((virus->filter)->array);
//        // Save new
//        (virus->filter)->array = bf_new_array;
//
//        token = strtok(NULL, SEP_TOKEN);
//    }
//
//}