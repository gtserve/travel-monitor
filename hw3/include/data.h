/* data.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW1_DATA_H
#define SYSPRO_HW1_DATA_H

#include "hashtable.h"
#include "bloomfilter.h"
#include "skiplist.h"
#include "pipe.h"


/* -------------------------------------- Data Types -------------------------------------------- */

typedef struct {
    char *name;
    unsigned int population;
    unsigned int pop_by_age[4];
} Country;

typedef struct {
    int answer;
    char *date;
    char *country_name;
} TravelRequest;

typedef struct {
    int id;
    char *first_name;
    char *last_name;
    Country *country;
    unsigned int age;
} Citizen;

typedef struct {
    Citizen *citizen;
    char *date;
} VaccinationType;

typedef struct {
    char *name;
    BloomFilter *filter;
    SkipList *vaccinated;
    SkipList *not_vaccinated;
    SkipList *vaccinations_by_date;
} VirusInfo;

typedef struct {
    int id;
    int buffer_size;
    unsigned int bloom_size;
    char in_dir_path[PATH_SIZE];
    unsigned int exp_records;
    HashTable *citizens;
    HashTable *countries;
    HashTable *viruses;
    HashTable *parsed_files;
} MonitorData;

typedef struct {
    HashTable *countries;
    HashTable *viruses;
    unsigned int bloom_size;
    unsigned int exp_records;
} TM_MonitorData;

typedef struct {
    int num_monitors;
    TM_MonitorData **mon_data;
    PipeChannel *pipe_channels;
    HashTable *country_to_monitor;
    HashTable *virus_to_requests;
} TM_Data;

/* ------------------------------------ Basic Operations ---------------------------------------- */

/* TravelRequest */
void trq_destroy(TravelRequest **travel_req);

/* VaccinationType */
void vac_destroy(VaccinationType **vaccination);

/* Country */
void cnt_destroy(Country **country);

/* Citizen */
void ctz_destroy(Citizen **citizen);

/* VirusInfo */
VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records);

void vir_destroy(VirusInfo **virus);

/* MonitorData */
MonitorData *mnd_create(unsigned int bloom_size, unsigned int exp_records);

void mnd_destroy(MonitorData **gen_data);

/* TM_MonitorData */
TM_MonitorData *tmm_create(unsigned int bloom_size, unsigned int exp_records);

void tmm_destroy(TM_MonitorData **tmm_data);

/* TM_Data */
TM_Data *tmd_create(int num_monitors, unsigned int bloom_size, unsigned int exp_records);

void tmd_destroy(TM_Data **tm_data);


#endif //SYSPRO_HW1_DATA_H
