#ifndef SYSPRO_HW2_TRAVELMONITOR_H
#define SYSPRO_HW2_TRAVELMONITOR_H

void travel_request(int citizen_id, char *date, char *country_from, char *country_to,
                    char *virus_name);

void travel_stats(char *virus_name, char *date1, char *date2, char *country_name);

void search_vacc_status(int citizen_id);

void add_vacc_records(char *country_dir);

void exit_monitors();

#endif //SYSPRO_HW2_TRAVELMONITOR_H
