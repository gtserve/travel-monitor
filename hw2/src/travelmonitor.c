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
#include "../include/handler.h"

#define PROG_NAME   "travelMonitor"
#define USAGE_STR   "Program Usage:\n %s [-m numMonitors] [-b bufferSize] [-s sizeOfBloom] " \
                    "[-i input_dir]\n"


#define PIPE_PERMS 0666
#define INT_STR_SIZE 20
#define EXP_RECORDS 1000


typedef struct {
    int num_monitors;
    int buffer_size;
    int bloom_size;
    char input_dir[PATH_SIZE];
} ProgArguments;

void usage(char *prog_name);

TM_Data *tm_data = NULL;

ProgArguments p_args = {0, 0, 0};

struct pollfd *pfds;



int main(int argc, char **argv) {

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

    // Find path to Monitor exe.
    char m_path[PATH_SIZE];
    if (getcwd(m_path, PATH_SIZE) == NULL) {
        perror("getcwd");
        exit(-1);
    }
    strcpy(m_path + strlen(m_path), "/monitor");

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

    // Create Data
    tm_data = tmd_create(p_args.num_monitors, p_args.bloom_size, EXP_RECORDS);

    tm_data->pipe_channels = (PipeChannel *) malloc(sizeof(PipeChannel) * p_args.num_monitors);

    // Save monitors process ids
    pid_t *monitor_pids = (pid_t *) malloc(sizeof(pid_t) * p_args.num_monitors);

    pfds = (struct pollfd *) malloc(sizeof(struct pollfd) * p_args.num_monitors);

    for (int i = 0; i < p_args.num_monitors; i++) {
        // Create Named Pipe (Reader)
        sprintf(tm_data->pipe_channels[i].reader_name, "%s/my_pipe_M%d_R", PIPE_PATH, i);
        if (mkfifo(tm_data->pipe_channels[i].reader_name, PIPE_PERMS) == -1) {
            perror(tm_data->pipe_channels[i].reader_name);
            exit(-1);
        }
        strcpy(m_args[5], tm_data->pipe_channels[i].reader_name);

        // Create Named Pipe (Writer)
        sprintf(tm_data->pipe_channels[i].writer_name, "%s/my_pipe_M%d_W", PIPE_PATH, i);
        if (mkfifo(tm_data->pipe_channels[i].writer_name, PIPE_PERMS) == -1) {
            perror(tm_data->pipe_channels[i].writer_name);
            exit(-1);
        }
        strcpy(m_args[6], tm_data->pipe_channels[i].writer_name);

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
            // Save Monitor ID.
            monitor_pids[i] = pid;
        }
    }

    // Open and initialize pipe channels.
    for (int i = 0; i < p_args.num_monitors; i++) {
        if ((tm_data->pipe_channels[i].writer_fd = open(tm_data->pipe_channels[i].writer_name, O_WRONLY)) == -1 ) {
            perror(tm_data->pipe_channels[i].writer_name);
            exit(-1);
        }

        if ((tm_data->pipe_channels[i].reader_fd = open(tm_data->pipe_channels[i].reader_name, O_RDONLY)) == -1 ) {
            perror(tm_data->pipe_channels[i].reader_name);
            exit(-1);
        }

        pfds[i].fd = tm_data->pipe_channels[i].reader_fd;
        pfds[i].events = POLLIN;
    }

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
        SEND_STR(CDIR, dent->d_name, tm_data->pipe_channels[i % p_args.num_monitors].writer_fd,
                 p_args.buffer_size);

        // Create new country
        Country *country = (Country *) malloc(sizeof(Country));
        STR_CPY(dent->d_name, country->name);
        country->population = 0;

        // Save country in Monitor Data
        htb_insert(tm_data->mon_data[i % p_args.num_monitors]->countries, dent->d_name,
                   STR_BYTES(dent->d_name), country);

        // Save country in TM Data
        int *m_id = (int *) malloc(sizeof(int));
        *m_id = i % p_args.num_monitors;
        htb_insert(tm_data->country_to_monitor, dent->d_name,
                   STR_BYTES(dent->d_name), m_id);
    }
    closedir(in_dir);

    // Send Directories is done.
    for (int i = 0; i < p_args.num_monitors; i++) {
        char *buffer = "DONE";
        SEND_STR(CDIRS_DONE, buffer, tm_data->pipe_channels[i].writer_fd,p_args.buffer_size);
    }

    // Get filters.
    int counter = 0;
    while ((counter < p_args.num_monitors) && (poll(pfds, p_args.num_monitors, -1))) {
        for (int i = 0; i < p_args.num_monitors; i++) {
            if (pfds[i].revents & POLLIN) {
                OP_CODE op_code;
                char *payload = NULL;
                char *del_ptr = NULL;

                RECEIVE(op_code, payload, tm_data->pipe_channels[i].reader_fd,
                        p_args.buffer_size, del_ptr);

                if (op_code == BFILTER) {
                    // Create new virus
                    VirusInfo *virus = vir_create(payload, p_args.bloom_size, EXP_RECORDS);
                    htb_insert(tm_data->mon_data[i]->viruses, payload, STR_BYTES(payload), virus);
                    free(del_ptr);

                    // Get Bloom filter array.
                    payload = NULL;
                    del_ptr = NULL;
                    RECEIVE(op_code, payload, tm_data->pipe_channels[i].reader_fd,
                            p_args.buffer_size, del_ptr);

                    // Copy filter array.
                    memcpy(virus->filter->array, payload, BF_ARR_BYTES(virus->filter));
                } else if (op_code == BFILTERS_DONE) {
                    counter++;
                } else {
                    perror("Unexpected Message");
                    exit(-1);
                }

                free(del_ptr);
            }
        }
    }

    // Handle commands from input.
    cmd_handler();

    // Wait for children
    for (int i = 0; i < p_args.num_monitors; i++)
        wait(&(monitor_pids[i]));

    // Close pipes.
    for (int i = 0; i < p_args.num_monitors; i++) {
        close(tm_data->pipe_channels[i].reader_fd);
        close(tm_data->pipe_channels[i].writer_fd);
    }

    // Destroy pipe channels.
    for (int i = 0; i < p_args.num_monitors; i++) {
        if (unlink(tm_data->pipe_channels[i].reader_name) == -1) {
            perror(tm_data->pipe_channels[i].reader_name);
            exit(-1);
        }

        if (unlink(tm_data->pipe_channels[i].writer_name) == -1) {
            perror(tm_data->pipe_channels[i].writer_name);
            exit(-1);
        }
    }

    // Destroy Monitor arguments.
    for (int i = 0; i < num_m_args; i++)
        free(m_args[i]);

    // Destroy Monitor data.
    tmd_destroy(&tm_data);

    // Destroy Monitor pids array.
    free(monitor_pids);

    printf("[TM]: DONE!\n");
    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}

void travel_request(int citizen_id, char *date, char *country_from, char *country_to,
                    char *virus_name) {

    int m_id = *((int *) htb_search(tm_data->country_to_monitor, country_from,
                          STR_BYTES(country_from)));

    VirusInfo *virus = htb_search(tm_data->mon_data[m_id]->viruses, virus_name,
                                  STR_BYTES (virus_name));

    TravelRequest *travel_req = (TravelRequest *) malloc(sizeof(TravelRequest));
    STR_CPY(date, travel_req->date);
    STR_CPY(country_to, travel_req->country_name);


    if (blf_query(virus->filter, citizen_id)) {

        SEND_STR(SL_QUERY, virus_name, tm_data->pipe_channels[m_id].writer_fd,
                 p_args.buffer_size);

        SEND_DATA(SL_QUERY, (char *) &citizen_id, sizeof(int),
                  tm_data->pipe_channels[m_id].writer_fd,p_args.buffer_size);

        OP_CODE op_code;
        char *payload = NULL;
        char *del_ptr = NULL;
        RECEIVE(op_code, payload, tm_data->pipe_channels[m_id].reader_fd,
                p_args.buffer_size, del_ptr);

        if (op_code == YES) {
            int vacc_days = date_to_days(payload);
            int travel_days = date_to_days(date);

            if (travel_days - vacc_days > 6 * 30) {
                printf("REQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE\n");
                travel_req->answer = 0;
            } else {
                printf("REQUEST ACCEPTED – HAPPY TRAVELS\n");
                travel_req->answer = 1;
            }
        } else {
            printf("REQUEST REJECTED - YOU ARE NOT VACCINATED\n");
            travel_req->answer = 0;
        }
        free(del_ptr);
    } else {
        printf("REQUEST REJECTED - YOU ARE NOT VACCINATED\n");
        travel_req->answer = 0;
    }

    SkipList *skiplist = htb_search(tm_data->virus_to_requests, virus_name,
                                    STR_BYTES(virus_name));
    if (skiplist == NULL) {
        skiplist = skl_create();
        htb_insert(tm_data->virus_to_requests, virus_name, STR_BYTES(virus_name), skiplist);
    }
    skl_insert(skiplist, compose_key(citizen_id, date), travel_req);
}

void travel_stats(char *virus_name, char *date1, char *date2, char *country_name) {

    SkipList *sl_requests = htb_search(tm_data->virus_to_requests, virus_name,
                                       STR_BYTES(virus_name));

    if (sl_requests == NULL) {
        printf("VIRUS DOESNT HAVE REQUESTS\n");
        return;
    }

    int date1_key = compose_key(0, date1);
    int date2_key = compose_key(9999, date2);

    int count_yes = 0;
    int count_no = 0;

    SLNode *cur_node = skl_get_next_node(sl_requests, date1_key);
    while ((cur_node) && cur_node->key <= date2_key) {
        TravelRequest *travel_req = cur_node->item;

        if ((country_name) && !STR_EQUALS(country_name, travel_req->country_name)) {
            cur_node = cur_node->next[0];
            continue;
        }

        if (travel_req->answer == 0) {
            count_no++;
        } else {
            count_yes++;
        }

        cur_node = cur_node->next[0];
    }

    printf("TOTAL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n",
           count_yes + count_no, count_yes, count_no);

}

void search_vacc_status(int citizen_id) {

    for (int i = 0; i < p_args.num_monitors; i++) {
        SEND_DATA(ID_QUERY, (char *) (&citizen_id), sizeof(int),
                  tm_data->pipe_channels[i].writer_fd,p_args.buffer_size);
    }

    OP_CODE op_code = -1;
    int count = 0;
    while (poll(pfds, p_args.num_monitors, -1)) {
        for (int i = 0; i < p_args.num_monitors; i++) {
            if (pfds[i].revents & POLLIN) {

                char *payload = NULL;
                char *del_ptr = NULL;
                RECEIVE(op_code, payload, tm_data->pipe_channels[i].reader_fd,
                        p_args.buffer_size, del_ptr);

                if (op_code == YES) {
                    printf("%s\n", payload);
                    free(del_ptr);
                    return;
                } else if (op_code == NO) {
                    count++;
                } else {
                    perror("search_vass_status");
                    exit(-1);
                }

                free(del_ptr);
                if (count == p_args.num_monitors)
                    return;
            }
        }
    }

}

void add_vacc_records(char *country_dir) {

    int m_id = *((int *) htb_search(tm_data->country_to_monitor, country_dir,
                                    STR_BYTES(country_dir)));

    SEND_STR(CDIR, country_dir, tm_data->pipe_channels[m_id].writer_fd,
             p_args.buffer_size);

    OP_CODE op_code = -1;
    while ((op_code != BFILTERS_DONE) && (poll(pfds, p_args.num_monitors, -1))) {
        if (pfds[m_id].revents & POLLIN) {
            char *payload = NULL;
            char *del_ptr = NULL;

            RECEIVE(op_code, payload, tm_data->pipe_channels[m_id].reader_fd,
                    p_args.buffer_size, del_ptr);

            if (op_code == BFILTERS_DONE) {
                free(del_ptr);
                return;
            }

            VirusInfo *virus = htb_search(tm_data->mon_data[m_id]->viruses, payload, STR_BYTES(payload));

            if (virus == NULL) {
                // Create new virus.
                vir_create(payload, p_args.bloom_size, EXP_RECORDS);
                // Save new virus.
                htb_insert(tm_data->mon_data[m_id]->viruses, payload, STR_BYTES(payload), virus);
            }
            free(del_ptr);

            // Get Bloom filter array.
            payload = NULL;
            del_ptr = NULL;
            RECEIVE(op_code, payload, tm_data->pipe_channels[m_id].reader_fd,
                    p_args.buffer_size, del_ptr);

            // Copy filter array.
            memcpy(virus->filter->array, payload, BF_ARR_BYTES(virus->filter));

            free(del_ptr);
        }
    }
}

void exit_monitors() {
    char *buffer = "DONE";
    for (int i = 0; i < p_args.num_monitors; i++) {
        SEND_STR(EXIT, buffer, tm_data->pipe_channels[i].writer_fd, p_args.buffer_size);
    }
}
