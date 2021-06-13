/* commands.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../include/util.h"
#include "../include/commands.h"

#define ERROR_IN_RECORD(rec, rec_len)                   \
{                                                       \
    str_replace((rec), (rec_len), '\0', ' ');           \
    (rec)[(rec_len) - 1] = '\n';                        \
    fprintf(stderr, "ERROR IN RECORD %s", (rec));       \
}


int vaccine_status_bloom(MonitorData *data, int citizen_id, char *virus_name) {
    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
    if (virus) {
        if (blf_query(virus->filter, citizen_id)) {
            printf("MAYBE\n");
        } else {
            printf("NOT VACCINATED\n");
        }
    } else {
        fprintf(stderr, "ERROR: VIRUS '%s' NOT IN DATA\n", virus_name);
        return -1;
    }

    return 0;
}

int insert_record(MonitorData *data, char *record) {

    unsigned long record_length = strlen(record);
    char *token = NULL;
    char *save_ptr;

    // ID
    if ((token = strtok_r(record, TOK_DELIM, &save_ptr)) == NULL) {
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
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *first_name = token;

    // Last Name
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *last_name = token;

    // Country
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alpha_or_c(token, '_')) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *country_name = token;

    // Age
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_int(token)) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    int age = (int) strtol(token, NULL, 10);

    // Virus
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    if (!str_is_alphanum_or_c(token, '_')) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }
    char *virus_name = token;

    // YES/NO
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
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
        if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) == NULL) {
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
    if ((token = strtok_r(NULL, TOK_DELIM, &save_ptr)) != NULL) {
        ERROR_IN_RECORD(record, record_length);
        return -1;
    }

    pthread_mutex_lock(&(data->lock));

    // Search Citizen ID in data.
    Citizen *citizen = htb_search(data->citizens, &id, sizeof(int));
    VirusInfo *virus = NULL;
    if (!citizen) {
        // Search country
        Country *country = htb_search(data->countries, country_name,
                                      STR_BYTES(country_name));
        if (!country) {
            // Create new country
            country = (Country *) malloc(sizeof(Country));
            STR_CPY(country_name, country->name);
            country->population = 0;
            for (int i = 0; i < 4; i++)
                country->pop_by_age[i] = 0;

            // Save new country
            htb_insert(data->countries, country_name,
                       STR_BYTES(country_name), country);
        }
        country->population++;
        country->pop_by_age[get_age_group(age)]++;

        // Create new citizen
        citizen = (Citizen *) malloc(sizeof(Citizen));
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

            pthread_mutex_unlock(&(data->lock));
            return -1;
        }

        // Search virus
        virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
        if (virus) {
            if (skl_search(virus->vaccinated, id) ||
                skl_search(virus->not_vaccinated, id)) {
                ERROR_IN_RECORD(record, record_length);

                pthread_mutex_unlock(&(data->lock));
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
        skl_insert(virus->vaccinated, id, vaccination);
        skl_insert(virus->vaccinations_by_date, compose_key(id, date), citizen);
    } else {
        // Save as not-vaccinated
        skl_insert(virus->not_vaccinated, id, citizen);
    }

    pthread_mutex_unlock(&(data->lock));
    return 0;
}