/* cmd_handler.c
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
#include <errno.h>

#include "../include/util.h"
#include "../include/commands.h"
#include "../include/handler.h"
#include "../include/tm_client.h"

enum cmd_code {
    EXIT,
    TRAVEL_REQUEST,
    TRAVEL_STATS,
    ADD_VACC_RECORDS,
    SEARCH_VACC_STATUS,
};


int get_command(char *buffer) {

    char *token = NULL;
    if ((token = strtok(buffer, TOK_DELIM)) == NULL)
        return -1;

    if (*token != '/') {
        fprintf(stderr, "Error: Commands should start with '/'.\n");
        return -1;
    }

    if (STR_EQUALS(token, "/exit")) {
        return EXIT;
    } else if (STR_EQUALS(token, "/travelRequest")) {
        return TRAVEL_REQUEST;
    } else if (STR_EQUALS(token, "/travelStats")) {
        return TRAVEL_STATS;
    } else if (STR_EQUALS(token, "/addVaccinationRecords")) {
        return ADD_VACC_RECORDS;
    } else if (STR_EQUALS(token, "/searchVaccinationStatus")) {
        return SEARCH_VACC_STATUS;
    }

    fprintf(stderr, "Error: Unknown command '%s'\n", token);
    return -1;
}

int get_id(char *buffer) {

    char *token = NULL;
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        fprintf(stderr, "Error: Citizen ID is not acceptable.\n");
        return -1;
    }

    if (!str_is_int(token)) {
        fprintf(stderr, "Error: Citizen ID '%s' is not an integer.\n", token);
        return -1;
    }

    int id = (int) strtol(token, NULL, 10);

    if ((id < 0) || (9999 < id)) {
        fprintf(stderr, "Error: Citizen IDs should be in range [0, 9999].\n");
        return -1;
    }

    return id;
}

int get_age(char *buffer) {
    char *token = NULL;
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        return -1;
    }

    if (!str_is_int(token)) {
        fprintf(stderr, "Error: Citizen age '%s' is not an integer.\n", token);
        return -1;
    }

    int age = (int) strtol(token, NULL, 10);

    if ((age < 0) || (120 < age)) {
        fprintf(stderr, "Error: Citizen age should be in range [0, 120].\n");
        return -1;
    }

    return age;
}

char *get_arg(char *buffer) {
    return strtok(NULL, TOK_DELIM);
}

/* -------------------------------------- Command Handler --------------------------------------- */
void cmd_handler() {

    char *buffer = NULL;
    size_t length = 0;
    size_t bytes;

    while ((bytes = getline(&buffer, &length, stdin)) != -1) {
        if (bytes == -1) {
            if (errno == EINVAL) {
                fprintf(stderr, "[CH] Error: getline() wrong arguments.\n");
                continue;
            } else if (errno == ENOMEM) {
                fprintf(stderr, "[CH] Error: getline() no memory.\n");
                continue;
            }
        }

        int code = get_command(buffer);

        if (code < 0)
            continue;

        switch (code) {
            case EXIT:
                free(buffer);
                log_file();
                exit_monitors();
                return;
            case TRAVEL_REQUEST: {
                int id;
                if ((id = get_id(buffer)) < 0)
                    continue;

                char *date = NULL;
                if ((date = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No date.\n");
                    continue;
                }
                if (!str_is_digit_or_c(date, '-')) {
                    fprintf(stderr, "ERROR: Invalid date.\n");
                    continue;
                }

                char *country_from = NULL;
                if ((country_from = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No country_from.\n");
                    continue;
                }

                char *country_to = NULL;
                if ((country_to = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No country_to.\n");
                    continue;
                }

                char *virus_name = NULL;
                if ((virus_name = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No virus_name.\n");
                    continue;
                }

                travel_request(id, date, country_from, country_to, virus_name);
                break;
            }
            case TRAVEL_STATS: {
                char *virus_name = NULL;
                if ((virus_name = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No virus_name.\n");
                    continue;
                }

                char *date1 = NULL;
                if ((date1 = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No date.\n");
                    continue;
                }

                char *date2 = NULL;
                if ((date2 = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No date.\n");
                    continue;
                }

                char *country = get_arg(buffer);

                travel_stats(virus_name, date1, date2, country);
                break;
            }
            case SEARCH_VACC_STATUS: {
                int id;
                if ((id = get_id(buffer)) < 0)
                    continue;

                search_vacc_status(id);
                break;
            }
            case ADD_VACC_RECORDS: {
                char *country_dir = NULL;
                if ((country_dir = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No country_dir.\n");
                    continue;
                }

                add_vacc_records(country_dir);
                break;
            }
            default:
                break;
        }
    }
}
