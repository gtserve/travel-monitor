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
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../include/bloomfilter.h"
#include "../include/pipe.h"
#include "../include/msg.h"
#include "../include/util.h"
#include "../include/data.h"
#include "../include/commands.h"
#include "../include/comm_protocol.h"
#include "../include/sockets.h"

MonitorData *mon_data;

int record_parser(char *records_file_name);

void send_filters();

void op_handler(OP_CODE code, char *payload);

void process_dir(char *dir_name);

void skiplist_query(char *virus_name);

void id_query(int citizen_id);

PipeChannel pc;



int main(int argc, char *argv[]) {
    /* Program Arguments:
     *   0: Executable name
     *   1: Monitor ID
     *   2: Buffer Size
     *   3: Bloom Filter size
     *   4: Input Directory path
     *   5: Pipe-Writer name
     *   6: Pipe-Reader name
     *   7: NULL
     */

    int id = (int) strtol(argv[1], NULL, 10);
    printf("[S%d]: Start!\n", id);

    /* Sockets */
    int port = MY_PORT;
    int socket_fd, newsocket_fd;

    struct sockaddr_in server;
    struct sockaddr_in client;
    struct sockaddr *server_ptr = (struct sockaddr *) &server;
    struct sockaddr *client_ptr = (struct sockaddr *) &client;
    socklen_t client_len;
//    socklen_t *cl_len_ptr = &(client_len);
    struct hostent *host = NULL;

    /* Create socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation");
        exit(-1);
    }

//    char host_name[NAME_SIZE];
//    gethostname(host_name, NAME_SIZE);
//    if ((host = gethostbyname(host_name)) == NULL) {
//        herror("gethostbyname");
//        exit(-1);
//    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    /* Bind socket. */
    if (bind(socket_fd, server_ptr, sizeof(server)) < 0) {
        perror("bind");
        close(socket_fd);
        exit(-1);
    }

    /* Listen socket. */
    if (listen(socket_fd, 5) < 0) {
        perror("bind");
        close(socket_fd);
        exit(-1);
    }

    printf("[S%d]: Listening for connections to port %d\n", id, port);

    client_len = sizeof(client);
    if ((newsocket_fd = accept(socket_fd, client_ptr, &client_len)) < 0) {
        perror("accept");
        close(socket_fd);
        exit(-1);
    }

//    if ((host = gethostbyaddr((char *) &(client.sin_addr.s_addr),
//                              sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
//        perror("gethostbyaddr");
//        exit(-1);
//    }

//    printf("[S%d]: Accepted connection from %s\n", id, host->h_name);

    printf("[S%d]: Accepted connection.\n", id);

    char *payload = NULL;
    char *del_ptr = NULL;
    int op_code = -1;

    RECEIVE(op_code, payload, newsocket_fd, BUF_SIZE, del_ptr);

    printf("[S%d]: Received %s\n", id, payload);

    free(del_ptr);

    char buffer[NAME_SIZE] = "Hello back!";

    SEND_STR(YES, buffer, newsocket_fd, BUF_SIZE);


    close(socket_fd);
    close(newsocket_fd);

//    int bloom_size = (int) strtol(argv[3], NULL, 10);
//    mon_data = mnd_create(bloom_size, EXP_RECORDS);
//
//    mon_data->id = (int) strtol(argv[1], NULL, 10);
//    mon_data->buffer_size = (int) strtol(argv[2], NULL, 10);
//    strcpy(mon_data->in_dir_path, argv[4]);
//
//    strcpy(pc.reader_name, argv[6]);
//    strcpy(pc.writer_name, argv[5]);
//
//    printf("[M%d]: Received '%s' and '%s'\n", mon_data->id, pc.reader_name, pc.writer_name);
//
//    if ((pc.reader_fd = open(pc.reader_name, O_RDONLY)) == -1) {
//        perror(pc.reader_name);
//        exit(-1);
//    }
//
//    if ((pc.writer_fd = open(pc.writer_name, O_WRONLY)) == -1) {
//        perror(pc.writer_name);
//        exit(-1);
//    }
//
//    struct pollfd pfds[1];
//    pfds[0].fd = pc.reader_fd;
//    pfds[0].events = POLLIN;

    // Get Directories
//    OP_CODE op_code = -1;
//    while ((op_code != CDIRS_DONE) && (poll(pfds, 1, -1))) {
//        if (pfds[0].revents & POLLIN) {
//            char *payload = NULL;
//            char *del_ptr = NULL;
//            RECEIVE(op_code, payload, pc.reader_fd, mon_data->buffer_size, del_ptr);
//            if (op_code == CDIR) {
//                process_dir(payload);
//            }
//            free(del_ptr);
//        }
//    }

    // Send Filters.
//    send_filters();

    // Print Countries
    //    HT_Iterator *ht_iter = htb_iter_create(mon_data->countries);
    //    Country *virus = NULL;
    //    while ((virus = htb_iter_next(ht_iter)) != NULL ) {
    //        printf("[M%d]: %s\n", mon_data->id, virus->name);
    //    }
    //    htb_iter_destroy(&ht_iter);


//    while (poll(pfds, 1, -1)) {
//        if (pfds[0].revents & POLLIN) {
//            OP_CODE op_code;
//            char *payload = NULL;
//            char *del_ptr = NULL;
//            RECEIVE(op_code, payload, pc.reader_fd, mon_data->buffer_size, del_ptr);
//            printf("[M%d]: %s\n", mon_data->id, payload);
//            op_handler(op_code, payload);
//            free(del_ptr);
//        }
//    }

    printf("[S%d]: Done!", id);
    return 0;
}


void op_handler(OP_CODE code, char *payload) {
    switch (code) {
        case EXIT: {
            close(pc.reader_fd);
            close(pc.writer_fd);
            printf("[M%d]: DONE!\n", mon_data->id);
            mnd_destroy(&mon_data);
            exit(0);
        }
        case CDIR: {
            process_dir(payload);
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
        default: {
            exit(-15);
        }
    }
}

int record_parser(char *records_file_name) {

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

        insert_record(mon_data, line);
        rec_count++;
    }

    fclose(records_file);
    free(line);
    return 0;
}

void send_filters() {
    // Send Bloom filters.
    HT_Iterator *ht_iter = htb_iter_create(mon_data->viruses);
    VirusInfo *virus = NULL;
    while ((virus = htb_iter_next(ht_iter)) != NULL) {
        // Send Virus name.
        SEND_STR(BFILTER, virus->name, pc.writer_fd, mon_data->buffer_size);

        // Send Bloom filter array.
        int ba_bytes = BF_ARR_BYTES(virus->filter);
        SEND_DATA(BFILTER, (char *) (virus->filter->array), ba_bytes, pc.writer_fd,
                  mon_data->buffer_size);
    }
    htb_iter_destroy(&ht_iter);

    char *buffer = "DONE";
    SEND_STR(BFILTERS_DONE, buffer, pc.writer_fd, mon_data->buffer_size);
}

void process_dir(char *dir_name) {

    Country *country = htb_search(mon_data->countries, dir_name, STR_BYTES(dir_name));

    if (country == NULL) {
        // Create new country
        country = (Country *) malloc(sizeof(Country));
        STR_CPY(dir_name, country->name);
        country->population = 0;
        for (int i = 0; i < 4; i++)
            country->pop_by_age[i] = 0;

        // Save new country
        htb_insert(mon_data->countries, dir_name, STR_BYTES(dir_name), country);
    }

    // Open directory.
    DIR *dir = NULL;
    char dir_path[PATH_SIZE] = "";
    strcat(dir_path, mon_data->in_dir_path);
    strcat(dir_path, "/");
    strcat(dir_path, dir_name);
    if ((dir = opendir(dir_path)) == NULL) {
        perror(dir_path);
        exit(-1);
    }

    struct dirent *dent = NULL;
    for (int j = 0; ((dent = readdir(dir)) != NULL); j++) {
        if (dent->d_type == DT_REG) {
            if (!htb_search(mon_data->parsed_files, dent->d_name,
                            STR_BYTES(dent->d_name))) {
                printf("[M%d]: New File %s\n", mon_data->id, dent->d_name);
                char file_path[PATH_SIZE] = "";
                strcat(file_path, dir_path);
                strcat(file_path, "/");
                strcat(file_path, dent->d_name);
                record_parser(file_path);
                int *x = (int *) malloc(sizeof(int));
                htb_insert(mon_data->parsed_files, dent->d_name,
                           STR_BYTES(dent->d_name), x);
            }
        }
    }
    closedir(dir);
}

void skiplist_query(char *virus_name) {

    OP_CODE op_code;
    char *payload = NULL;
    char *del_ptr = NULL;
    RECEIVE(op_code, payload, pc.reader_fd, mon_data->buffer_size, del_ptr);

    int citizen_id = *((int *) payload);

//    printf("[M%d]: %d\n", mon_data->id, citizen_id);


    VirusInfo *virus = htb_search(mon_data->viruses, virus_name, STR_BYTES(virus_name));

    if (virus == NULL) {
        printf("[M%d]: VIRUS NULL!\n", mon_data->id);
        exit(-1);
    }

    char *buffer = "NO";

    VaccinationType *vacc  = NULL;
    if ((vacc = skl_search(virus->vaccinated, citizen_id)) != NULL) {
        SEND_STR(YES, vacc->date, pc.writer_fd, mon_data->buffer_size);
    } else {
        SEND_STR(NO, buffer, pc.writer_fd, mon_data->buffer_size);
    }

    free(del_ptr);
}

void id_query(int citizen_id) {


    Citizen *citizen = htb_search(mon_data->citizens, &citizen_id, sizeof(int));
    char string[1024];


    if (citizen != NULL) {
        sprintf(string, "%d %s %s %s\nAGE %d\n", citizen->id, citizen->first_name,
                citizen->last_name, citizen->country->name, citizen->age);

        int offset = (int) strlen(string);
        VirusInfo *virus = NULL;
        HT_Iterator *ht_iter = htb_iter_create(mon_data->viruses);
        while ((virus = htb_iter_next(ht_iter)) != NULL) {

            VaccinationType *vacc  = NULL;
            if ((vacc = skl_search(virus->vaccinated, citizen_id)) != NULL) {
                sprintf(string + offset, "%s VACCINATED ON %s", virus->name, vacc->date);
            } else if (skl_search(virus->not_vaccinated, citizen_id) != NULL) {
                sprintf(string + offset, "%s NOT YET VACCINATED", virus->name);
            }

        }
        htb_iter_destroy(&ht_iter);

        SEND_STR(YES, string, pc.writer_fd, mon_data->buffer_size);

    } else {
        SEND_STR(NO, string, pc.writer_fd, mon_data->buffer_size);
        return;
    }
}