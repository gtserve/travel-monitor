/* cmd_handler.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/util.h"
#include "../include/commands.h"
#include "../include/handler.h"

enum cmd_code {
    EXIT,
    VACCINE_STATUS_BLOOM,
    VACCINE_STATUS,
    POPULATION_STATUS,
    POP_STATUS_BY_AGE,
    INSERT_CITIZEN_RECORD,
    VACCINATE_NOW,
    LIST_NON_VACCINATED
};


int get_command(char *buffer) {

    char *token = NULL;
    if ((token = strtok(buffer, TOK_DELIM)) == NULL) {
        fprintf(stderr, "Error: Command '%s' is not acceptable.\n", token);
        return -1;
    }

    if (*token != '/') {
        fprintf(stderr, "Error: Commands should start with '/'.\n");
        return -1;
    }

    if (STR_EQUALS(token, "/exit")) {
        return EXIT;
    } else if (STR_EQUALS(token, "/vaccineStatusBloom")) {
        return VACCINE_STATUS_BLOOM;
    } else if (STR_EQUALS(token, "/vaccineStatus")) {
        return VACCINE_STATUS;
    } else if (STR_EQUALS(token, "/populationStatus")) {
        return POPULATION_STATUS;
    } else if (STR_EQUALS(token, "/popStatusByAge")) {
        return POP_STATUS_BY_AGE;
    } else if (STR_EQUALS(token, "/insertCitizenRecord")) {
        return INSERT_CITIZEN_RECORD;
    } else if (STR_EQUALS(token, "/vaccinateNow")) {
        return VACCINATE_NOW;
    } else if (STR_EQUALS(token, "/list-nonVaccinated-Persons")) {
        return LIST_NON_VACCINATED;
    }

    fprintf(stderr, "Error: Unknown command '%s'\n", token);
    return -1;
}

int get_id(char *buffer) {

    // TODO: REmove Error from strtok
    char *token = NULL;
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        fprintf(stderr, "Error: Citizen ID '%s' is not acceptable.\n", token);
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

/* ---------------------------- Command Handler ----------------------------- */

void cmd_handler(GeneralData *data) {

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
                return;
            case VACCINE_STATUS_BLOOM: {
                int id;
                if ((id = get_id(buffer)) < 0)
                    continue;

                char *virus_name = NULL;
                if ((virus_name = get_arg(buffer)) == NULL) {
                    fprintf(stderr, "ERROR: No virus name.\n");
                    continue;
                }

                vaccine_status_bloom(data, id, virus_name);
                break;
            }
            case VACCINE_STATUS: {
                int id = -1;
                if ((id = get_id(buffer)) < 0)
                    continue;

                char *virus_name = get_arg(buffer);

                vaccine_status(data, id, virus_name);
                break;
            }
            case POPULATION_STATUS: {
                char *country_name = NULL;
                char *virus_name = NULL;
                char *date1 = NULL;
                char *date2 = NULL;

                char *arg1 = get_arg(buffer);
                if (htb_search(data->countries, arg1, STR_BYTES(arg1))) {
                    country_name = arg1;
                    arg1 = get_arg(buffer);
                }
                if (htb_search(data->viruses, arg1, STR_BYTES(arg1))) {
                    virus_name = arg1;
                } else {
                    fprintf(stderr, "ERROR: '%s' NOT IN DATA\n", arg1);
                    break;
                }

                date1 = get_arg(buffer);
                date2 = get_arg(buffer);

                if (!date1 || !date2) {
                    fprintf(stderr, "ERROR: INVALID DATE RANGE\n");
                    break;
                }
                if (!str_is_digit_or_c(date1, '-') || !str_is_digit_or_c(date2, '-')) {
                    fprintf(stderr, "ERROR: INVALID DATE FORMAT\n");
                    break;
                }

                populationStatus(data, country_name, virus_name, date1, date2);
                break;
            }
            case POP_STATUS_BY_AGE: {
                char *country_name = NULL;
                char *virus_name = NULL;
                char *date1 = NULL;
                char *date2 = NULL;

                char *arg1 = get_arg(buffer);
                if (htb_search(data->countries, arg1, STR_BYTES(arg1))) {
                    country_name = arg1;
                    arg1 = get_arg(buffer);
                }
                if (htb_search(data->viruses, arg1, STR_BYTES(arg1))) {
                    virus_name = arg1;
                } else {
                    fprintf(stderr, "ERROR: '%s' NOT IN DATA\n", arg1);
                    break;
                }

                date1 = get_arg(buffer);
                date2 = get_arg(buffer);

                if (!date1 || !date2) {
                    fprintf(stderr, "ERROR: INVALID DATE RANGE\n");
                    break;
                }
                if (!str_is_digit_or_c(date1, '-') || !str_is_digit_or_c(date2, '-')) {
                    fprintf(stderr, "ERROR: INVALID DATE FORMAT\n");
                    break;
                }

                popStatusByAge(data, country_name, virus_name, date1, date2);
                break;
            }
            case INSERT_CITIZEN_RECORD:
                insert_record(data, (buffer +
                                     STR_BYTES("/insertCitizenRecord")));
                break;
            case VACCINATE_NOW: {
                int id = -1;
                if ((id = get_id(buffer)) < 0)
                    continue;

                char *first_name = get_arg(buffer);
                char *last_name = get_arg(buffer);
                char *country_name = get_arg(buffer);
                int age = get_age(buffer);
                char *virus_name = get_arg(buffer);

                vaccinate_now(data, id, first_name, last_name, country_name,
                              age, virus_name);
                break;
            }
            case LIST_NON_VACCINATED: {
                char *virus_name = get_arg(buffer);
                list_not_vaccinated(data, virus_name);
                break;
            }
            default:
                break;
        }
    }
}
