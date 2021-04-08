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

#define STR_CPY(src, dest)                                          \
{                                                                   \
    dest = (char *) malloc(sizeof(char) * (strlen((src)) + 1));     \
    strcpy((dest), (src));                                          \
}                                                                   \


VirusInfo * search_virus(VirusInfo **viruses, char *virus_name) {
    int i = 0;
    while ((i < MAGIC_SIZE) && (viruses[i])) {
        if (strcmp(virus_name, viruses[i]->name) == 0) {
            return viruses[i];
        }
        i++;
    }
    return NULL;
}

VirusInfo * insert_virus(GeneralData *data, char *virus_name) {
    int i = 0;
    while ((i < MAGIC_SIZE) && (data->viruses[i])) { i++; }
    if (i == MAGIC_SIZE)
        return NULL;

    data->viruses[i] = vir_create(virus_name, data->bloom_size, data->exp_records);
    return data->viruses[i];
}


VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records) {
    VirusInfo *v_info = (VirusInfo *) malloc(sizeof(VirusInfo));
    v_info->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(v_info->name, name);
    v_info->filter = blf_create(exp_records, bloom_size);
    v_info->vaccinated_list = skl_create();
    v_info->not_vaccinated_list = skl_create();

    return v_info;
}

void vir_insert(VirusInfo *virus, CitizenType *citizen, int vaccinated) {
    if (vaccinated == 0) {
        skl_insert(virus->not_vaccinated_list, citizen);
    } else {
        blf_add(virus->filter, citizen->id);
        skl_insert(virus->vaccinated_list, citizen);
    }
}

void vir_vaccinate(VirusInfo *virus, CitizenType *citizen) {
    if (skl_search(virus->vaccinated_list, citizen)) {
        printf("ERROR: Vaccinated!\n");
    } else {
        if (skl_search(virus->not_vaccinated_list, citizen)) {
            skl_delete(virus->not_vaccinated_list, citizen->id);
        }
        vir_insert(virus, citizen, 1);
    }
}

void vir_destroy(VirusInfo **virus) {
    free((*virus)->name);
    blf_destroy(&(*virus)->filter);
    skl_destroy(&(*virus)->vaccinated_list);
    skl_destroy(&(*virus)->not_vaccinated_list);
    free(*virus);
    *virus = NULL;
}

int record_parser(char *rec_file_name, GeneralData *data) {
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

        CitizenType *citizen = (CitizenType *) malloc(sizeof(CitizenType));
        citizen->id = id;
        STR_CPY(firstName, citizen->firstName);
        STR_CPY(lastName, citizen->lastName);
        citizen->country = NULL;
        citizen->age = age;
        data->citizens[line_index] = citizen;

        VirusInfo *virus = search_virus(data->viruses, virus_name);
        if (!virus)
            virus = insert_virus(data, virus_name);
        vir_insert(virus, citizen, vaccinated);

        line_index++;
    }

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
