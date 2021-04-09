/* util.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/util.h"

#define TOK_DELIM " \n"

#define STR_BYTES(str) (strlen(str) + 1)
#define STR_CPY(src, dest)                                          \
{                                                                   \
    dest = (char *) malloc(sizeof(char) * STR_BYTES(src));          \
    strcpy((dest), (src));                                          \
}                                                                   \


int record_parser(char *rec_file_name, GeneralData *gen_data) {
    // 889 John Papadopoulos Greece 52 COVID-19 YES 27-12-2020
    // 776 Maria Tortellini Italy 36 SARS-1 NO

    FILE *rec_file = fopen(rec_file_name, "r");
    char *line = NULL;
    size_t len = 0;
    int line_index = 0;
    while ((getline(&line, &len, rec_file)) != -1) {

        //TODO: ERROR STRING CHECK RECORD

        char *token = strtok(line, TOK_DELIM);
        int id = (int) strtol(token, NULL, 10);

        token = strtok(NULL, TOK_DELIM);
        char *firstName = token;

        token = strtok(NULL, TOK_DELIM);
        char *lastName = token;

        token = strtok(NULL, TOK_DELIM);
        char *country_name = token;

        token = strtok(NULL, TOK_DELIM);
        int age = (int) strtol(token, NULL, 10);

        token = strtok(NULL, TOK_DELIM);
        char *virus_name = token;

        token = strtok(NULL, TOK_DELIM);
        int vaccinated = strcmp(token, "YES") == 0;

        char *date = NULL;
        if (vaccinated) {
            token = strtok(NULL, TOK_DELIM);
            date = token;
        }

        //TODO: ERROR LOGIC CHECK RECORD

        // Search citizen in data.
        if (htb_search(gen_data->citizens, &id, sizeof(int))) {
            // Ignore record.
            //printf("[RP] Record with ID: %d is already inserted!\n", id);
            continue;
        }

        // Create new citizen.
        CitizenType *citizen = (CitizenType *) malloc(sizeof(CitizenType));
        citizen->id = id;
        STR_CPY(firstName, citizen->firstName);
        STR_CPY(lastName, citizen->lastName);
        citizen->country = NULL;
        citizen->age = age;

        // Insert new citizen.
        htb_insert(gen_data->citizens, &citizen->id, sizeof(int), citizen);

        // Search virus in data.
        VirusInfo *virus = NULL;
        if ((virus = htb_search(gen_data->viruses, virus_name, STR_BYTES(virus_name)))) {
            //printf("[RP] Virus '%s' is already inserted!\n", virus_name);
        } else {
            // Create new virus.
            virus = vir_create(virus_name, gen_data->bloom_size, gen_data->exp_records);
            // Insert new virus.
            htb_insert(gen_data->viruses, virus_name, STR_BYTES(virus_name), virus);
        }
        vir_insert(virus, citizen, vaccinated);

        line_index++;
    }

    free(line);

    return 0;
}


int input_handler(char *input) {

    char *token = strtok(input, TOK_DELIM);

    if (strcmp(token, "/exit") == 0) {
        return -1;
    } else if (strcmp(token, "/vaccineStatusBloom") == 0) {

    } else if (strcmp(token, "/vaccineStatus") == 0) {

    } else if (strcmp(token, "/populationStatus") == 0) {

    } else if (strcmp(token, "/popStatusByAge") == 0) {

    } else if (strcmp(token, "/insertCitizenRecord") == 0) {

    } else if (strcmp(token, "/vaccinateNow") == 0) {

    } else if (strcmp(token, "/list-nonVaccinated-Persons") == 0) {

    }

    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, TOK_DELIM);
    }

    return 0;
}
