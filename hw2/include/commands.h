/* commands.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_COMMANDS_H
#define SYSPRO_HW1_COMMANDS_H

#include "data.h"


int vaccine_status_bloom(MonitorData *data, int citizen_id, char *virus_name);

int vaccine_status(MonitorData *data, int citizen_id, char *virus_name);

int populationStatus(MonitorData *data, char *country_name, char *virus_name,
                     char *date1, char *date2);

int popStatusByAge(MonitorData *data, char *country_name, char *virus_name,
                   char *date1, char *date2);

int insert_record(MonitorData *data, char *record);

int vaccinate_now(MonitorData *data, int id, char *first_name, char *last_name,
                  char *country_name, int age, char *virus_name);

int list_not_vaccinated(MonitorData *data, char *virus_name);

#endif //SYSPRO_HW1_COMMANDS_H
