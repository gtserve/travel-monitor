/* tm_server.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>

#include "../include/bloomfilter.h"
#include "../include/msg.h"
#include "../include/util.h"
#include "../include/data.h"
#include "../include/commands.h"
#include "../include/comm_protocol.h"
#include "../include/thpool.h"


typedef struct {
    int id;
    int socket_buf_size;
    int cyclic_buf_size;
    int bloom_size;
    char input_dir[PATH_SIZE];
    int port;
    int num_threads;
} MonitorArgs;


MonitorData *m_data = NULL;
ServerData *s_data = NULL;
FD_Channel channel;
ThreadPool *th_pool = NULL;

void record_parser(char *records_file_name);

void send_filters();

void op_handler(OP_CODE code, char *payload);

void process_dir(char *dir_name);

void skiplist_query(char *virus_name);

void id_query(int citizen_id);

void log_file();


int main(int argc, char *argv[]) {
    /* Program Arguments:
     *   0: Executable name
     *   1: Monitor ID
     *   2: Socket Buffer Size
     *   3: Cyclic Buffer Size
     *   4: Bloom Filter size
     *   5: Input Directory path
     *   6: Port
     *   7: Number of Threads
     *   8: NULL
     */

    int bloom_size = (int) strtol(argv[4], NULL, 10);
    m_data = mnd_create(bloom_size, EXP_RECORDS);

    m_data->id = (int) strtol(argv[1], NULL, 10);
    printf("[S%d]: Process started! PID=%d\n", m_data->id, getpid());

    m_data->socket_buf_size = (int) strtol(argv[2], NULL, 10);
    m_data->cyclic_buf_size = (int) strtol(argv[3], NULL, 10);
    strcpy(m_data->in_dir_path, argv[5]);
    m_data->num_threads = (int) strtol(argv[7], NULL, 10);

    int port = (int) strtol(argv[6], NULL, 10);
    s_data = sdt_create(port);

    /* Wait connection from Client. */
    accept_connection(s_data);
    printf("[S%d]: Accepted connection from %s, port=%d\n", m_data->id, s_data->host->h_name,
           s_data->channel.port);

    /* Establish Comm-Channel. */
    channel.reader_fd = s_data->channel.socket_fd;
    channel.writer_fd = s_data->channel.socket_fd;


    struct pollfd pfds[1];
    pfds[0].fd = channel.reader_fd;
    pfds[0].events = POLLIN;

    /* Create ThreadPool */
    th_pool = thp_create(m_data->num_threads, m_data->cyclic_buf_size);


    /* Get Directories */
    OP_CODE op_code = UNKNOWN;
    while ((op_code != CDIRS_DONE) && (poll(pfds, 1, -1))) {
        if (pfds[0].revents & POLLIN) {
            char *payload = NULL;
            char *del_ptr = NULL;
            RECEIVE(op_code, payload, channel.reader_fd, m_data->socket_buf_size, del_ptr);
            if (op_code == CDIR) {
                process_dir(payload);
            }
            free(del_ptr);
        }
    }

    /* Wait for all threads to finish tasks. */
    thp_block_until_done(th_pool);

    /* Send Bloom Filters. */
    send_filters();

//    // Print Countries
//    HT_Iterator *ht_iter = htb_iter_create(m_data->countries);
//    Country *virus = NULL;
//    while ((virus = htb_iter_next(ht_iter)) != NULL) {
//        printf("[S%d]: %s\n", m_data->id, virus->name);
//    }
//    htb_iter_destroy(&ht_iter);

    /* Handle Operations from Parent-Client. */
    while (poll(pfds, 1, -1)) {
        if (pfds[0].revents & POLLIN) {
            op_code = UNKNOWN;
            char *payload = NULL;
            char *del_ptr = NULL;
            RECEIVE(op_code, payload, channel.reader_fd, m_data->socket_buf_size, del_ptr);
            op_handler(op_code, payload);
            free(del_ptr);
        }
    }

    return 0;
}


void op_handler(OP_CODE code, char *payload) {
    switch (code) {
        case EXIT: {
            printf("[S%d]: Done!\n", m_data->id);
            log_file();

            /* Destroy Data */
            thp_destroy(&th_pool);
            sdt_destroy(&s_data);
            mnd_destroy(&m_data);

            exit(0);
        }
        case CDIR: {
            process_dir(payload);
            thp_block_until_done(th_pool);
            send_filters();
            break;
        }
        case CDIRS_DONE: {
            // Do Nothing.
            break;
        }
        case SL_QUERY: {
            skiplist_query(payload);
            break;
        }
        case ID_QUERY: {
            int id = *((int *) payload);
            id_query(id);
            break;
        }
        case UNKNOWN: {
            perror_exit("Unknown OP_CODE");
        }
        default: {
            exit(-15);
        }
    }
}

void record_parser(char *records_file_name) {

    FILE *records_file = NULL;
    if ((records_file = fopen(records_file_name, "r")) == NULL) {
        perror(records_file_name);
        exit(-1);
    }
    int rec_count = 0;

    char *line = NULL;
    size_t line_length = 0;
    size_t bytes;
    while ((bytes = getline(&line, &line_length, records_file)) != -1) {
        if (bytes == -1) {
            if (errno == EINVAL) {
                fprintf(stderr, "[RP] ERROR: getline() wrong args.\n");
                continue;
            } else if (errno == ENOMEM) {
                fprintf(stderr, "[RP] ERROR: getline() no memory.\n");
                continue;
            }
        }

        insert_record(m_data, line);
        rec_count++;
    }

    fclose(records_file);
    free(records_file_name);
    free(line);
}

void send_filters() {
    // Send Bloom filters.
    HT_Iterator *ht_iter = htb_iter_create(m_data->viruses);
    VirusInfo *virus = NULL;
    while ((virus = htb_iter_next(ht_iter)) != NULL) {
        // Send Virus name.
        SEND_STR(BFILTER, virus->name, channel.writer_fd, m_data->socket_buf_size);

        // Send Bloom filter array.
        int ba_bytes = BF_ARR_BYTES(virus->filter);
        SEND_DATA(BFILTER, (char *) (virus->filter->array), ba_bytes, channel.writer_fd,
                  m_data->socket_buf_size);
    }
    htb_iter_destroy(&ht_iter);

    char *buffer = "DONE";
    SEND_STR(BFILTERS_DONE, buffer, channel.writer_fd, m_data->socket_buf_size);
}

void process_dir(char *dir_name) {

    /* Search country in data. */
    Country *country = htb_search(m_data->countries, dir_name, STR_BYTES(dir_name));

    if (country == NULL) {
        /* Create new country. */
        country = (Country *) malloc(sizeof(Country));
        STR_CPY(dir_name, country->name);
        country->population = 0;
        for (int i = 0; i < 4; i++)
            country->pop_by_age[i] = 0;

        /* Save new country. */
        htb_insert(m_data->countries, dir_name, STR_BYTES(dir_name), country);
    }

    /* Open directory. */
    DIR *dir = NULL;
    char dir_path[PATH_SIZE] = "";
    strcat(dir_path, m_data->in_dir_path);
    strcat(dir_path, "/");
    strcat(dir_path, dir_name);
    if ((dir = opendir(dir_path)) == NULL) {
        perror_exit("opendir");
    }

    struct dirent *dent = NULL;
    for (int j = 0; ((dent = readdir(dir)) != NULL); j++) {
        if (dent->d_type == DT_REG) {
            if (!htb_search(m_data->parsed_files, dent->d_name, STR_BYTES(dent->d_name))) {
                // printf("[S%d]: New File %s\n", m_data->id, dent->d_name);

                char file_path[PATH_SIZE] = "";
                strcat(file_path, dir_path);
                strcat(file_path, "/");
                strcat(file_path, dent->d_name);

                char *new_file_path = NULL;
                STR_CPY(file_path, new_file_path);

                /* Parse file. */
                // record_parser(file_path);

                thp_push_task(th_pool, (task_func_ptr) record_parser, new_file_path);

                /* Save file as parsed. */
                int *x = (int *) malloc(sizeof(int));
                htb_insert(m_data->parsed_files, dent->d_name, STR_BYTES(dent->d_name), x);
            }
        }
    }

    closedir(dir);
}

void skiplist_query(char *virus_name) {

    OP_CODE op_code;
    char *payload = NULL;
    char *del_ptr = NULL;
    RECEIVE(op_code, payload, channel.reader_fd, m_data->socket_buf_size, del_ptr);

    int citizen_id = *((int *) payload);
    free(del_ptr);

    RECEIVE(op_code, payload, channel.reader_fd, m_data->socket_buf_size, del_ptr);

    char *trav_req_date = payload;

//    printf("[S%d]: %d\n", m_data->id, citizen_id);

    VirusInfo *virus = htb_search(m_data->viruses, virus_name, STR_BYTES(virus_name));

    if (virus == NULL) {
        printf("[S%d]: VIRUS NULL!\n", m_data->id);
        exit(-1);
    }

    char *buffer = "NO";

    m_data->num_req_total++;

    VaccinationType *vacc = NULL;
    if ((vacc = skl_search(virus->vaccinated, citizen_id)) != NULL) {
        SEND_STR(YES, vacc->date, channel.writer_fd, m_data->socket_buf_size);

        int vacc_days = date_to_days(vacc->date);
        int travel_days = date_to_days(trav_req_date);

        if (travel_days - vacc_days > 6 * 30) {
//            printf("REQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE\n");
            m_data->num_req_rejected++;
        } else {
//            printf("REQUEST ACCEPTED – HAPPY TRAVELS\n");
            m_data->num_req_accepted++;
        }
    } else {
        SEND_STR(NO, buffer, channel.writer_fd, m_data->socket_buf_size);

//        printf("REQUEST REJECTED - YOU ARE NOT VACCINATED\n");
        m_data->num_req_rejected++;
    }

    free(del_ptr);
}

void id_query(int citizen_id) {


    Citizen *citizen = htb_search(m_data->citizens, &citizen_id, sizeof(int));
    char string[1024];


    if (citizen != NULL) {
        sprintf(string, "%d %s %s %s\nAGE %d\n", citizen->id, citizen->first_name,
                citizen->last_name, citizen->country->name, citizen->age);

        int offset = (int) strlen(string);
        VirusInfo *virus = NULL;
        HT_Iterator *ht_iter = htb_iter_create(m_data->viruses);
        while ((virus = htb_iter_next(ht_iter)) != NULL) {

            VaccinationType *vacc = NULL;
            if ((vacc = skl_search(virus->vaccinated, citizen_id)) != NULL) {
                sprintf(string + offset, "%s VACCINATED ON %s", virus->name, vacc->date);
            } else if (skl_search(virus->not_vaccinated, citizen_id) != NULL) {
                sprintf(string + offset, "%s NOT YET VACCINATED", virus->name);
            }

        }
        htb_iter_destroy(&ht_iter);

        SEND_STR(YES, string, channel.writer_fd, m_data->socket_buf_size);

    } else {
        SEND_STR(NO, string, channel.writer_fd, m_data->socket_buf_size);
        return;
    }
}

void log_file() {

    /* Find current working dir file_path. */
    char file_path[PATH_SIZE];
    sprintf(file_path, "../logs/log_file.%d", getpid());

    FILE *file = NULL;
    if ((file = fopen(file_path, "w+")) == NULL) {
        perror_exit("log file open");
    }

    char str[5120] = {0};

    HT_Iterator *ht_iter = htb_iter_create(m_data->countries);
    Country *country = NULL;
    while ((country = htb_iter_next(ht_iter)) != NULL) {
        strcat(str, country->name);
        strcat(str, "\n");
    }
    htb_iter_destroy(&ht_iter);

    sprintf(str + strlen(str), "TOTAL TRAVEL REQUESTS %d\nACCEPTED %d\nREJECTED %d\n",
            m_data->num_req_total, m_data->num_req_accepted, m_data->num_req_rejected);

    fputs(str, file);

    fclose(file);
}