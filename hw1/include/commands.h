/* commands.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_COMMANDS_H
#define SYSPRO_HW1_COMMANDS_H

#include "data.h"

int vaccine_status_bloom(GeneralData *data, int citizen_id, char *virus_name);

int vaccine_status(GeneralData *data, int citizen_id, char *virus_name);

int insert_record(GeneralData *data, char *record);

int vaccinate_now(GeneralData *data, int id, char *first_name, char *last_name,
                  char *country_name, int age, char *virus_name);

int list_not_vaccinated(GeneralData *data, char *virus_name);

#endif //SYSPRO_HW1_COMMANDS_H
