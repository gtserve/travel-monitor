/* commands.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/util.h"
#include "../include/commands.h"

#define ERROR_IN_RECORD(rec, rec_len)                   \
{                                                       \
    str_replace((rec), (rec_len), '\0', ' ');           \
    (rec)[(rec_len) - 1] = '\n';                        \
    fprintf(stderr, "ERROR IN RECORD %s", (rec));       \
}


int vaccine_status_bloom(GeneralData *data, int citizen_id, char *virus_name) {
    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
    if (virus) {
        if (blf_query(virus->filter, citizen_id)) {
            printf("MAYBE\n");
        } else {
            printf("NOT VACCINATED\n");
        }
    } else {
        printf("VIRUS '%s' NOT IN DATA\n", virus_name);
        return -1;
    }

    return 0;
}

int vaccine_status(GeneralData *data, int citizen_id, char *virus_name) {
    CitizenType *citizen = htb_search(data->citizens, &citizen_id, sizeof(int));
    if (!citizen) {
        printf("CITIZEN WITH ID %d NOT IN DATA\n", citizen_id);
        return -1;
    }

    if (virus_name) {
        VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
        if (virus) {
            VaccinationType *vaccination = skl_search(virus->vaccinated_list, citizen_id);
            if (vaccination) {
                printf("VACCINATED ON %s\n", vaccination->date);
            } else {
                printf("NOT VACCINATED\n");
            }
        } else {
            printf("VIRUS '%s' NOT IN DATA\n", virus_name);
            return -1;
        }
    } else {
        HT_Iterator *iter = htb_iter_create(data->viruses);

        VirusInfo *virus = NULL;
        while ((virus = htb_iter_next(iter))) {
            VaccinationType *vaccination = skl_search(virus->vaccinated_list, citizen_id);
            if (vaccination) {
                printf("%s YES %s\n", virus->name, vaccination->date);
            } else {
                citizen = skl_search(virus->not_vaccinated_list, citizen_id);
                if (citizen) {
                    printf("%s NO\n", virus->name);
                }
            }
        }
        htb_iter_destroy(&iter);
    }

    return 0;
}


int insert_record(GeneralData *data, char *record) {

    unsigned long record_length = strlen(record);
    char *token = NULL;

    // ID
    if ((token = strtok(record, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_int(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    int id = (int) strtol(token, NULL, 10);
    if ((id < 0) || (9999 < id)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }

    // First Name
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *first_name = token;

    // Last Name
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *last_name = token;

    // Country
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha_or_c(token, '_')) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *country_name = token;

    // Age
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_int(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    int age = (int) strtol(token, NULL, 10);

    // Virus
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha_or_c(token, '_')) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *virus_name = token;

    // YES/NO
    if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!(STR_EQUALS(token, "YES") || STR_EQUALS(token, "NO"))) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *vaccinated = token;

    // Date
    char *date = NULL;
    if (STR_EQUALS(vaccinated, "YES")) {
        if ((token = strtok(NULL, TOK_DELIM)) == NULL) {
            ERROR_IN_RECORD(record, record_length);
            return -1;
        }
        if (!str_is_digit_or_c(token, '-')) {
            ERROR_IN_RECORD(record, record_length);
            return -1;
        }
        date = token;
    }

    // Record junk
    if ((token = strtok(NULL, TOK_DELIM)) != NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }

    // Search Citizen ID in data.
    CitizenType *citizen = htb_search(data->citizens, &id, sizeof(int));
    VirusInfo *virus = NULL;
    if (!citizen) {
        // Search country
        CountryType *country = htb_search(data->countries, country_name,
                                          STR_BYTES(country_name));
        if (!country) {
            // Create new country
            country = (CountryType *) malloc(sizeof(CountryType));
            STR_CPY(country_name, country->name);
            country->population = 0;

            // Save new country
            htb_insert(data->countries, country_name,
                       STR_BYTES(country_name), country);
        }
        country->population++;

        // Create new citizen
        citizen = (CitizenType *) malloc(sizeof(CitizenType));
        citizen->id = id;
        STR_CPY(first_name, citizen->first_name);
        STR_CPY(last_name, citizen->last_name);
        citizen->country = country;
        citizen->age = age;

        // Save new citizen.
        htb_insert(data->citizens, &(citizen->id), sizeof(int), citizen);

        // Search virus
        virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
        if (!virus) {
            // Create new virus
            virus = vir_create(virus_name, data->bloom_size, data->exp_records);

            // Save new virus
            htb_insert(data->viruses, virus_name,
                       STR_BYTES(virus_name), virus);
        }
    } else {

        // Must have same attributes
        if (!(STR_EQUALS(first_name, citizen->first_name) &&
              STR_EQUALS(last_name, citizen->last_name) &&
              STR_EQUALS(country_name, citizen->country->name) &&
              age == citizen->age)) {
            ERROR_IN_RECORD(record, record_length);
            return -1;
        }

        // Search virus
        virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
        if (virus) {
            if (skl_search(virus->vaccinated_list, id) ||
                skl_search(virus->not_vaccinated_list, id)) {
                ERROR_IN_RECORD(record, record_length);
                return -1;
            }
        } else {
            // Create new virus
            virus = vir_create(virus_name, data->bloom_size, data->exp_records);

            // Save new virus
            htb_insert(data->viruses, virus_name,
                       STR_BYTES(virus_name), virus);
        }
    }

    if (STR_EQUALS(vaccinated, "YES")) {
        // Create new vaccination
        VaccinationType *vaccination = (VaccinationType *) malloc(sizeof(VaccinationType));
        vaccination->citizen = citizen;
        STR_CPY(date, vaccination->date);

        // Vaccinate citizen
        blf_add(virus->filter, id);
        skl_insert(virus->vaccinated_list, id, vaccination);
    } else {
        // Save as not-vaccinated
        skl_insert(virus->not_vaccinated_list, id, citizen);
    }

    return 0;
}

int vaccinate_now(GeneralData *data, int id, char *first_name, char *last_name,
                  char *country_name, int age, char *virus_name) {
    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
    if (!virus) {
        // Create new virus
        virus = vir_create(virus_name, data->bloom_size, data->exp_records);

        // Save new virus
        htb_insert(data->viruses, virus_name,
                   STR_BYTES(virus_name), virus);
    }

    VaccinationType *vaccination = skl_search(virus->vaccinated_list, id);
    if (vaccination) {
        printf("ERROR: CITIZEN %d ALREADY VACCINATED ON %s\n", id, vaccination->date);
        return -1;
    }

    char *todays_date = get_todays_date();

    // Search citizen in data.
    CitizenType *citizen = htb_search(data->citizens, &id, sizeof(int));
    if (!citizen) {
        char buffer[200];
        sprintf(buffer, "%d %s %s %s %d %s YES %s", id, first_name,
                last_name, country_name, age, virus_name, todays_date);
        insert_record(data, buffer);
        free(todays_date);
        return 0;
    }

    // Create new vaccination
    vaccination = (VaccinationType *) malloc(sizeof(VaccinationType));
    vaccination->citizen = citizen;
    STR_CPY(todays_date, vaccination->date);

    // Vaccinate citizen
    blf_add(virus->filter, id);
    skl_insert(virus->vaccinated_list, id, vaccination);
    if (skl_search(virus->not_vaccinated_list, citizen->id)) {
        skl_delete(virus->not_vaccinated_list, citizen->id);
    }
    free(todays_date);

    return 0;
}

int list_not_vaccinated(GeneralData *data, char *virus_name) {
    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
    if (!virus) {
        printf("VIRUS '%s' NOT IN DATA\n", virus_name);
        return -1;
    }

    CitizenType *citizen = NULL;
    SLNode *cur_node = virus->not_vaccinated_list->heads[0];
    while (cur_node) {
        citizen = cur_node->item;
        printf("%d %s %s %s %d\n", citizen->id, citizen->first_name, citizen->last_name,
               citizen->country->name, citizen->age);
        cur_node = cur_node->next[0];
    }

    return 0;
}