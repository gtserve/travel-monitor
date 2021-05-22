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
#include <dirent.h>
#include <errno.h>
#include <math.h>

#include "../include/pipe.h"
#include "../include/msg.h"
#include "../include/util.h"
#include "../include/data.h"
#include "../include/commands.h"
#include "../include/comm_protocol.h"

MonitorData *mon_data;

int record_parser(char *records_file_name);

int get_directories(PipeChannel pc, char ***dir_names, int monitor_id);

void send_filters(MonitorData *gen_data, PipeChannel pc, int bloom_size, int monitor_id);

void monitor_op_handler(OP_CODE op_code, char *payload);

void process_dir(char *dir_name);


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

    int bloom_size = (int) strtol(argv[3], NULL, 10);
    mon_data = gdt_create(bloom_size, 10000);

    mon_data->id = (int) strtol(argv[1], NULL, 10);
    mon_data->buffer_size = (int) strtol(argv[2], NULL, 10);
    strcpy(mon_data->in_dir_path, argv[4]);

    PipeChannel pc;
    strcpy(pc.reader_name, argv[6]);
    strcpy(pc.writer_name, argv[5]);

    printf("[M%d]: Received '%s' and '%s'\n", mon_data->id, pc.reader_name, pc.writer_name);

    if ((pc.reader_fd = open(pc.reader_name, O_RDONLY)) == -1) {
        perror(pc.reader_name);
        exit(-1);
    }

    if ((pc.writer_fd = open(pc.writer_name, O_WRONLY)) == -1) {
        perror(pc.writer_name);
        exit(-1);
    }

    // Get Directories
//    OP_CODE op_code;
//    while ((msg_get(pc.reader_fd, mon_data->buffer_size, &payload)) &&
//                ((op_code = decode_op(&payload)) != CDIRS_DONE)) {
//        monitor_op_handler(op_code, payload);
//    }

    struct pollfd pfds[1];
    pfds[0].fd = pc.reader_fd;
    pfds[0].events = POLLIN;

    OP_CODE op_code = -1;
    while ((op_code != CDIRS_DONE) && (poll(pfds, 1, -1))) {
        if (pfds[0].revents & POLLIN) {
            char *payload = NULL;
            msg_get(pc.reader_fd, mon_data->buffer_size, &payload);
            op_code = decode_op(&payload);
            monitor_op_handler(op_code, payload);
            free(payload);
        }
    }

    HT_Iterator *ht_iter = htb_iter_create(mon_data->countries);
    CountryType *country = NULL;
    while ((country = htb_iter_next(ht_iter)) != NULL ) {
        printf("[M%d]: %s\n", mon_data->id, country->name);
    }
    htb_iter_destroy(&ht_iter);


//    char *buffer = NULL;
//    int msg_bytes = msg_get(pc.reader_fd, 8, &buffer);
//
//    printf("%s + M%d|msg.bytes= %d\n", buffer, mon_data->id, msg_bytes);


//    // Get directories from travelmonitor
//    char **dir_names = NULL;
//    int num_dirs = get_directories(pc, &dir_names, mon_data->id);
//
//    printf("[M%d]: Countries[%d]= ", mon_data->id, num_dirs);
//    for (int i = 0; i < num_dirs; i++)
//        printf("%s ", dir_names[i]);
//    printf("\n");
//
//    // Create data
//    MonitorData *gen_data = gdt_create(bloom_size, 10000);
//    for (int i = 0; i < num_dirs; i++) {
//        DIR* dir = NULL;
//        char dir_path[PATH_SIZE];
//        sprintf(dir_path, "%s/%s", argv[2], dir_names[i]);
//        if ((dir = opendir(dir_path)) == NULL) {
//            perror(dir_path);
//            exit(-1);
//        }
//
//        char file_path[PATH_SIZE];
//        struct dirent *dent = NULL;
//        for (int j = 0; ((dent = readdir(dir)) != NULL); j++) {
//            if (dent->d_type == DT_REG) {
//                printf("[M%d]: File %s\n", mon_data->id, dent->d_name);
//                sprintf(file_path, "%s/%s", dir_path, dent->d_name);
//                record_parser(file_path);
//            }
//        }
//        closedir(dir);
//    }
//
//    send_filters(gen_data, pc, bloom_size, id);

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

//    for (int i = 0; i < num_dirs; i++)
//        free(dir_names[i]);
//    free(dir_names);

    printf("[M%d]: DONE!\n", mon_data->id);

    gdt_destroy(&mon_data);

    return 0;
}

//int get_directories(PipeChannel pc, char ***dir_names, int monitor_id) {
//
//    char *buffer;
//    msg_get(pc.reader_fd, 0, &buffer);
//
//    printf("[M%d]: %s\n", monitor_id, buffer);
//
//    int num_dirs = 0;
//    for (int i = 0; i < strlen(buffer); i++) {
//        if (buffer[i] == '$') {
//            num_dirs++;
//        }
//    }
//
//    printf("[M%d]: NUM_DIRS= %d\n", monitor_id, num_dirs);
//
//    *dir_names = (char **) malloc(num_dirs * sizeof(char *));
//
//    char *token = strtok(buffer, SEP_TOKEN);
//    int offset = 0;
//    for (int i = 0; token != NULL; i++) {
//        printf("[M%d]: Token%d= %s\n", monitor_id, i, token);
//        (*dir_names)[i] = (char *) malloc((int) strlen(token) + 1);
//        offset += (int) strlen(token) + 1;
//        printf("[M%d]: Buffer= %s, Offset= %d\n", monitor_id, buffer+offset, offset);
//        strcpy((*dir_names)[i], token);
//        printf("[M%d]: Dir%d= %s\n", monitor_id, i, (*dir_names)[i]);
//        token = strtok(NULL, SEP_TOKEN);
//    }
//
//    printf("[M%d]: NUM_DIRS= %d\n", monitor_id, num_dirs);
//
//
//    free(buffer);
//    return num_dirs;
//}
//
//void send_filters(MonitorData *gen_data, PipeChannel pc, int bloom_size, int monitor_id) {
//
//    int bf_bytes = (int) (ceil(bloom_size / (double) BAR_TYPE_BITS) * sizeof(BAR_TYPE));
//    int msg_bytes = ((gen_data->viruses)->num_entries) * bf_bytes;
//
//
//    printf("[M%d]: Num_Viruses=%d\n", monitor_id, ((gen_data->viruses)->num_entries));
//
//    HT_Iterator *ht_iter = htb_iter_create(gen_data->viruses);
//    VirusInfo *virus = NULL;
//    while ((virus = htb_iter_next(ht_iter)) != NULL) {
//        msg_bytes += (int) (sizeof(int) + strlen(virus->name) + 3);
//    }
//    htb_iter_destroy(&ht_iter);
//
//
//    char message[msg_bytes + 1];
//
//
//    char *filters_msg = (char *) malloc(msg_bytes);
//
//    ht_iter = htb_iter_create(gen_data->viruses);
//    virus = NULL;
//    int offset = 0;
//    printf("[M%d]: BF_MSG= ", monitor_id);
//    while ((virus = htb_iter_next(ht_iter)) != NULL) {
//        int virus_name_len = (int) strlen(virus->name);
//
//        printf("[M%d]: OK!\n", monitor_id);
//
//        // Virus name length
//        memcpy(filters_msg + offset, &virus_name_len, sizeof(int));
//        offset += sizeof(int);
//        printf("%d|", virus_name_len);
//
//        // Virus name
//        memcpy(filters_msg + offset, virus->name, virus_name_len);
//        offset += (int) virus_name_len;
//        printf("%s|", virus->name);
//
//        // Virus bloom filter array
//        memcpy(filters_msg + offset, (virus->filter)->array, bf_bytes);
//        offset += bf_bytes;
//        printf("%d||", bf_bytes);
//
//        char buffer[bf_bytes + 1];
//        memcpy(buffer, (virus->filter)->array, bf_bytes);
//        buffer[bf_bytes] = '\0';
//        offset += sprintf(message + offset, "%d$%s$%s$", virus_name_len,
//                          virus->name, buffer);
//    }
//    htb_iter_destroy(&ht_iter);
//    printf("%s\n", message);
//    fflush(stdin);
//
//    msg_send(pc.writer_fd, 0, message, msg_bytes + 1);
//
//    free(filters_msg);
//}

void monitor_op_handler(OP_CODE op_code, char *payload) {
    switch (op_code) {
        case CDIR: {
            process_dir(payload);
        } case CDIRS_DONE: {
            // Do Nothing.
            break;
        } default: {
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

void process_dir(char *dir_name) {

    // Create new country
    CountryType *country = (CountryType *) malloc(sizeof(CountryType));
    STR_CPY(dir_name, country->name);
    country->population = 0;
    for (int i = 0; i < 4; i++)
        country->pop_by_age[i] = 0;

    // Save new country
    htb_insert(mon_data->countries, dir_name, STR_BYTES(dir_name), country);

    DIR* dir = NULL;
    char dir_path[PATH_SIZE];
    sprintf(dir_path, "%s/%s", mon_data->in_dir_path, dir_name);
    if ((dir = opendir(dir_path)) == NULL) {
        perror(dir_path);
        exit(-1);
    }

    char file_path[PATH_SIZE];
    struct dirent *dent = NULL;
    for (int j = 0; ((dent = readdir(dir)) != NULL); j++) {
        if (dent->d_type == DT_REG) {
            printf("[M%d]: File %s\n", mon_data->id, dent->d_name);
            sprintf(file_path, "%s/%s", dir_path, dent->d_name);
            record_parser(file_path);
        }
    }
    closedir(dir);
}
