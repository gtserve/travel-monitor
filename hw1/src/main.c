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

#define PROG_NAME "vaccineMonitor"
#define ERR_FOPEN "Error: Couldn't open file '%s'.\n"
#define USAGE_STR "Program Usage:\n" \
                  "%s [-c citizenRecordsFile] [-b bloomSize]\n"

typedef struct {
    char *c_value;
    char *b_value;
} OptionsType;

void usage(char *prog_name);


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

    gdt_destroy(&gen_data);

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}
