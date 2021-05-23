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
#include <time.h>

#include "../include/util.h"
#include "../include/commands.h"
#include "../include/skiplist.h"

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

//int vaccine_status(MonitorData *data, int citizen_id, char *virus_name) {
//    Citizen *citizen = htb_search(data->citizens, &citizen_id, sizeof(int));
//    if (!citizen) {
//        fprintf(stderr, "ERROR: CITIZEN WITH ID %d NOT IN DATA\n", citizen_id);
//        return -1;
//    }
//
//    if (virus_name) {
//        VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
//        if (virus) {
//            VaccinationType *vaccination = skl_search(virus->vaccinated, citizen_id);
//            if (vaccination) {
//                printf("VACCINATED ON %s\n", vaccination->date);
//            } else {
//                printf("NOT VACCINATED\n");
//            }
//        } else {
//            fprintf(stderr, "VIRUS '%s' NOT IN DATA\n", virus_name);
//            return -1;
//        }
//    } else {
//        HT_Iterator *iter = htb_iter_create(data->viruses);
//
//        VirusInfo *virus = NULL;
//        while ((virus = htb_iter_next(iter))) {
//            VaccinationType *vaccination = skl_search(virus->vaccinated, citizen_id);
//            if (vaccination) {
//                printf("%s YES %s\n", virus->name, vaccination->date);
//            } else {
//                citizen = skl_search(virus->not_vaccinated, citizen_id);
//                if (citizen) {
//                    printf("%s NO\n", virus->name);
//                }
//            }
//        }
//        htb_iter_destroy(&iter);
//    }
//
//    return 0;
//}

//int populationStatus(MonitorData *data, char *country_name, char *virus_name,
//                     char *date1, char *date2) {
//
//    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
//    if (!virus) {
//        fprintf(stderr, "ERROR: VIRUS '%s' NOT IN DATA\n", virus_name);
//        return -1;
//    }
//
//    int date1_key = compose_key(0, date1);
//    int date2_key = compose_key(9999, date2);
//
//    HashTable *country_vacc = htb_create(10);
//
//    SLNode *cur_node = skl_get_next_node(virus->vaccinations_by_date, date1_key);
//    while (cur_node->key <= date2_key) {
//        Citizen *citizen = cur_node->item;
//        Country *country = citizen->country;
//
//        if ((country_name) && !STR_EQUALS(country_name, country->name)) {
//            cur_node = cur_node->next[0];
//            continue;
//        }
//
//        int *vacc_count = htb_search(country_vacc, country->name,
//                                     STR_BYTES(country->name));
//        if (!vacc_count) {
//            vacc_count = malloc(sizeof(int));
//            *vacc_count = 1;
//            htb_insert(country_vacc, country->name,
//                       STR_BYTES(country->name), vacc_count);
//        } else {
//            (*vacc_count)++;
//        }
//
//        cur_node = cur_node->next[0];
//    }
//
//    if ((country_name) && !(htb_search(country_vacc, country_name,
//                                       STR_BYTES(country_name)))) {
//        printf("%s 0 0.00%%\n", country_name);
//    }
//
//    HT_Iterator *iter = htb_iter_create(country_vacc);
//
//    EntryType *cur_entry = NULL;
//    while ((cur_entry = htb_iter_next_entry(iter))) {
//        char *c_name = cur_entry->key;
//        int *c_vacc_count = cur_entry->item;
//        Country *cur_country = htb_search(data->countries, c_name,
//                                              STR_BYTES(c_name));
//        double vacc_per = 100 * (*c_vacc_count /(double) cur_country->population);
//        printf("%s %d %.2lf%% \n", c_name, *c_vacc_count, vacc_per);
//    }
//
//    htb_iter_destroy(&iter);
//
//    htb_destroy(&country_vacc, 1);
//    return 0;
//}
//
//int popStatusByAge(MonitorData *data, char *country_name, char *virus_name,
//                   char *date1, char *date2) {
//
//    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
//    if (!virus) {
//        fprintf(stderr, "ERROR: VIRUS '%s' NOT IN DATA\n", virus_name);
//        return -1;
//    }
//
//    int date1_key = compose_key(0, date1);
//    int date2_key = compose_key(9999, date2);
//
//    HashTable *country_vacc = htb_create(10);
//
//    SLNode *cur_node = skl_get_next_node(virus->vaccinations_by_date, date1_key);
//    while (cur_node->key <= date2_key) {
//        Citizen *citizen = cur_node->item;
//        Country *country = citizen->country;
//
//        if ((country_name) && !STR_EQUALS(country_name, country->name)) {
//            cur_node = cur_node->next[0];
//            continue;
//        }
//
//        int *vacc_count = htb_search(country_vacc, country->name,
//                                     STR_BYTES(country->name));
//        if (!vacc_count) {
//            vacc_count = (int *) malloc(sizeof(int) * 4);
//            for (int i = 0; i < 4; i++)
//                vacc_count[i] = 0;
//            vacc_count[get_age_group((int) citizen->age)]++;
//            htb_insert(country_vacc, country->name,
//                       STR_BYTES(country->name), vacc_count);
//        } else {
//            vacc_count[get_age_group((int) citizen->age)]++;
//        }
//
//        cur_node = cur_node->next[0];
//    }
//
//    if ((country_name) && !(htb_search(country_vacc, country_name,
//                                       STR_BYTES(country_name)))) {
//        printf("%s\n", country_name);
//        printf("0-20 0 0.00%%\n20-40 0 0.00%%\n40-60 0 0.00%%\n60+   0 0.00%%\n\n");
//    }
//
//    HT_Iterator *iter = htb_iter_create(country_vacc);
//
//    EntryType *cur_entry = NULL;
//    while ((cur_entry = htb_iter_next_entry(iter))) {
//        char *c_name = cur_entry->key;
//        int *c_vacc_count = cur_entry->item;
//        Country *cur_country = htb_search(data->countries, c_name,
//                                              STR_BYTES(c_name));
//
//        printf("%s\n", c_name);
//        for (int i = 0; i < 4; i++) {
//            double vacc_per = 100 * (c_vacc_count[i] /(double) cur_country->pop_by_age[i]);
//            if (i == 3) {
//                printf("60+   ");
//            } else {
//                printf("%d-%d ", i * 20, (i + 1) * 20);
//            }
//            printf("%d %.2lf%%\n", c_vacc_count[i], vacc_per);
//        }
//        printf("\n");
//    }
//
//    htb_iter_destroy(&iter);
//
//    htb_destroy(&country_vacc, 1);
//    return 0;
//
//}

int insert_record(MonitorData *data, char *record) {

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
    if (!str_is_alphanum_or_c(token, '_')) {
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
            return -1;
        }

        // Search virus
        virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
        if (virus) {
            if (skl_search(virus->vaccinated, id) ||
                skl_search(virus->not_vaccinated, id)) {
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
        skl_insert(virus->vaccinated, id, vaccination);
        skl_insert(virus->vaccinations_by_date, compose_key(id, date), citizen);
    } else {
        // Save as not-vaccinated
        skl_insert(virus->not_vaccinated, id, citizen);
    }

    return 0;
}

//int vaccinate_now(MonitorData *data, int id, char *first_name, char *last_name,
//                  char *country_name, int age, char *virus_name) {
//    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
//    if (!virus) {
//        // Create new virus
//        virus = vir_create(virus_name, data->bloom_size, data->exp_records);
//
//        // Save new virus
//        htb_insert(data->viruses, virus_name,
//                   STR_BYTES(virus_name), virus);
//    }
//
//    VaccinationType *vaccination = skl_search(virus->vaccinated, id);
//    if (vaccination) {
//        fprintf(stderr, "ERROR: CITIZEN %d ALREADY VACCINATED ON %s\n", id, vaccination->date);
//        return -1;
//    }
//
//    char *todays_date = get_todays_date();
//
//    char buffer[200];
//    sprintf(buffer, "%d %s %s %s %d %s YES %s\n", id, first_name,
//            last_name, country_name, age, virus_name, todays_date);
//
//    // Search citizen in data.
//    Citizen *citizen = htb_search(data->citizens, &id, sizeof(int));
//    if (!citizen) {
//        insert_record(data, buffer);
//        free(todays_date);
//        return 0;
//    }
//
//    // Must have same attributes
//    if (!(STR_EQUALS(first_name, citizen->first_name) &&
//          STR_EQUALS(last_name, citizen->last_name) &&
//          STR_EQUALS(country_name, citizen->country->name) &&
//          age == citizen->age)) {
//        fprintf(stderr, "ERROR: CREDENTIALS MISMATCH FOR ID %d\n", id);
//        free(todays_date);
//        return -1;
//    }
//
//    // Create new vaccination
//    vaccination = (VaccinationType *) malloc(sizeof(VaccinationType));
//    vaccination->citizen = citizen;
//    STR_CPY(todays_date, vaccination->date);
//
//    // Vaccinate citizen
//    blf_add(virus->filter, id);
//    skl_insert(virus->vaccinated, id, vaccination);
//    skl_insert(virus->vaccinations_by_date, compose_key(id, todays_date), citizen);
//    if (skl_search(virus->not_vaccinated, citizen->id)) {
//        skl_delete(virus->not_vaccinated, citizen->id);
//    }
//    free(todays_date);
//
//    return 0;
//}
//
//int list_not_vaccinated(MonitorData *data, char *virus_name) {
//    VirusInfo *virus = htb_search(data->viruses, virus_name, STR_BYTES(virus_name));
//    if (!virus) {
//        fprintf(stderr, "ERROR: VIRUS '%s' NOT IN DATA\n", virus_name);
//        return -1;
//    }
//
//    Citizen *citizen = NULL;
//    SLNode *cur_node = virus->not_vaccinated->heads[0];
//    while (cur_node) {
//        citizen = cur_node->item;
//        printf("%d %s %s %s %d\n", citizen->id, citizen->first_name, citizen->last_name,
//               citizen->country->name, citizen->age);
//        cur_node = cur_node->next[0];
//    }
//
//    return 0;
//}