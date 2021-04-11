/* main.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "../include/util.h"
#include "../include/commands.h"
#include "../include/handler.h"

#define PROG_NAME "vaccineMonitor"
#define USAGE_STR "Program Usage:\n" \
                  "%s [-c citizenRecordsFile] [-b bloomSize]\n"

// Error messages
#define ERR_MSG_FOPEN "ERROR: Couldn't open file '%s'.\n"
#define ERR_MSG_NOT_INT "ERROR: Bloom size '%s' must be an integer.\n"
#define ERR_MSG_NOT_POS "ERROR: Bloom size '%s' must be positive.\n"
#define ERR_MSG_ARG "ERROR: Option -%c requires an argument.\n"
#define ERR_MSG_UNKNOWN "ERROR: Unknown option -%c.\n"
#define ERR_MSG_NOT_PRINT "ERROR: Unknown option character '\\x%x'.\n"

typedef struct {
    char *record_file;
    int bloom_size;
} OptionsType;

void usage(char *prog_name);

int record_parser(char *records_file_name, GeneralData *gen_data);


int main(int argc, char **argv) {

    OptionsType opt = {NULL, 0};
    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, "c:b:")) != EOF) {
        // Options: [-c citizenRecordsFile] [ -b bloomSize]
        switch (c) {
            case 'c':
                if (access(optarg, F_OK)) {
                    fprintf(stderr, ERR_MSG_FOPEN, optarg);
                    exit(EXIT_FAILURE);
                }
                opt.record_file = optarg;
                break;
            case 'b':
                if (!str_is_int(optarg)) {
                    fprintf(stderr, ERR_MSG_NOT_INT, optarg);
                    exit(EXIT_FAILURE);
                }
                if ((opt.bloom_size = (int) strtol(optarg,
                                                   NULL, 10)) < 0) {
                    fprintf(stderr, ERR_MSG_NOT_POS, optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                if ((optopt == 'c') || (optopt == 'b')) {
                    fprintf(stderr, ERR_MSG_ARG, optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, ERR_MSG_UNKNOWN, optopt);
                } else {
                    fprintf(stderr, ERR_MSG_NOT_PRINT, optopt);
                }
                usage(argv[0]);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    printf("RECORD_FILE: %s\n", opt.record_file);
    printf("BLOOM_SIZE:  %d\n", opt.bloom_size);

    int exp_records = 10000;

    GeneralData *gen_data = gdt_create(opt.bloom_size, exp_records);

    // Parse records and save them in data.
    record_parser(opt.record_file, gen_data);

    // Handle commands from input.
    cmd_handler(gen_data);

    gdt_destroy(&gen_data);
    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}

int record_parser(char *records_file_name, GeneralData *gen_data) {

    FILE *records_file = fopen(records_file_name, "r");
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

        insert_record(gen_data, line);

        rec_count++;
    }

    fclose(records_file);
    free(line);
    return 0;
}
