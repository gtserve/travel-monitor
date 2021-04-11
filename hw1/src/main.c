/* main.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "../include/util.h"
#include "../include/commands.h"
#include "../include/handler.h"

#define PROG_NAME "vaccineMonitor"
#define ERR_FOPEN "Error: Couldn't open file '%s'.\n"
#define USAGE_STR "Program Usage:\n" \
                  "%s [-c citizenRecordsFile] [-b bloomSize]\n"

typedef struct {
    char *c_value;
    char *b_value;
} OptionsType;

void usage(char *prog_name);

int record_parser(char *rec_file_name, GeneralData *gen_data);


int main(int argc, char **argv) {

    OptionsType options = {NULL, NULL};
    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, "c:b:")) != EOF) {
        // Options: [-c citizenRecordsFile] [ -b bloomSize]
        switch (c) {
            case 'c':
                options.c_value = optarg;
                if (!(fopen(options.c_value, "r"))) {
                    fprintf(stderr, ERR_FOPEN, options.c_value);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                options.b_value = optarg;
                break;
            case '?':
                if ((optopt == 'c') || (optopt == 'b')) {
                    fprintf(stderr, "Error: Option -%c requires an " \
                            "argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Error: Unknown option '-%c'.\n",
                            optopt);
                } else {
                    fprintf(stderr, "Error: Unknown option character " \
                            "'\\x%x'.\n", optopt);
                }
                usage(argv[0]);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    printf("Passed arguments:\n");
    printf("c: %s\n", options.c_value);
    printf("b: %s\n", options.b_value);

    int bloom_size = (int) strtol(options.b_value, NULL, 10);
    int exp_records = 10000;

    GeneralData *gen_data = gdt_create(bloom_size, exp_records);

    record_parser(options.c_value, gen_data);

    handler(gen_data);

    gdt_destroy(&gen_data);

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}

int record_parser(char *rec_file_name, GeneralData *gen_data) {
    // 889 John Papadopoulos Greece 52 COVID-19 YES 27-12-2020
    // 776 Maria Tortellini Italy 36 SARS-1 NO

    FILE *rec_file = fopen(rec_file_name, "r");
    char *line = NULL;
    size_t line_length = 0;
    int line_index = 0;
    size_t bytes_read = 0;
    while ((bytes_read = getline(&line, &line_length, rec_file)) != -1) {
        insert_record(gen_data, line);
        line_index++;
    }

    free(line);

    return 0;
}
