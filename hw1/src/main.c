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
#define ERR_FOPEN "Error: Couldn't open file '%s'."
#define USAGE_STR "Program Usage:\n" \
                  "%s [-c citizenRecordsFile] [-b bloomSize]\n"

#define MAX_CMD_ARGS 9
#define MAX_CMD_ARG_SIZE 100

// Options: [-c citizenRecordsFile] [ -b bloomSize]
typedef struct {
    int c_flag;
    int b_flag;
    char *c_value;
    char *b_value;
} OptionsType;

void usage(char *prog_name);


int main(int argc, char **argv) {

    OptionsType options = {0, 0, NULL, NULL};

    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, "c:b:")) != EOF) {
        switch (c) {
            case 'c':
                options.c_flag = 1;
                options.c_value = optarg;
                if (!(fopen(optarg, "r"))) {
                    fprintf(stderr, ERR_FOPEN, optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                options.b_flag = 1;
                options.b_value = optarg;
                break;
            case '?':
                if ((optopt == 'c') || (optopt == 'b')) {
                    fprintf(stderr,"Error: Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr,"Error: Unknown option character '\\x%x'.\n", optopt);
                }
                usage(argv[0]);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    printf("c: %d, %s\n", options.c_flag, options.c_value);
    printf("b: %d, %s\n", options.b_flag, options.b_value);

    for (int i = optind; i < argc; i++) {
        printf("Non option argument %s.\n", argv[i]);
    }

    char cmd_str[50] = {""};
    char cmd_args[150] = {""};

    int num_args = 0;

    while (strcmp(cmd_str, "exit") != 0) {

        num_args = scanf("/%s", cmd_str);

        printf("matches  = %d\n", num_args);
        printf("CMD_STR  = %s\n", cmd_str);
        printf("CMD_ARGS = %s\n", cmd_args);
    }

    size_t buffer_size = 150;
    char *buffer = (char *) malloc(sizeof(char) * buffer_size);

    while (getline(&buffer, &buffer_size, stdin) != -1) {
        if (input_handler(buffer) == -1) {
            break;
        }
    }

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}