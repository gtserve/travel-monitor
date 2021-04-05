/* main.c
 * ------------------------------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define PROG_NAME "vaccineMonitor"
#define OPT_STR   "c:b:"
#define USAGE_STR "Program Usage:\n  %s [-c citizenRecordsFile] [-b bloomSize]\n"
#define ERR_FOPEN "Cannot open file '%s'.\nPlease use a valid file name.\n"

// Options: [-c citizenRecordsFile] [ -b bloomSize]
typedef struct {
    int  c_flag;
    int  b_flag;
    char *c_value;
    char *b_value;
} OptionsType;

void usage(char *prog_name);


int main(int argc, char **argv) {

    OptionsType options = {0, 0, NULL, NULL};

    opterr = 0;

    int c;
    while ((c = getopt(argc, argv, OPT_STR)) != EOF) {
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
                    fprintf(stderr,"Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr,"Unknown option character '\\x%x'.\n", optopt);
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

    return 0;
}

void usage(char *prog_name) {
    fprintf(stderr, USAGE_STR, (prog_name ? prog_name : PROG_NAME));
    exit(EXIT_FAILURE);
}