/* util.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "../include/util.h"

#define TOK_DELIM " \n"


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
