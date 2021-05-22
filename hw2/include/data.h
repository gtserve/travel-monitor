/* data.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW1_DATA_H
#define SYSPRO_HW1_DATA_H

#include "hashtable.h"
#include "bloomfilter.h"
#include "skiplist.h"


/* -------------------------------------- Data Types -------------------------------------------- */

typedef struct {
    char *name;
    unsigned int population;
    unsigned int pop_by_age[4];
} CountryType;

typedef struct {
    int id;
    char *first_name;
    char *last_name;
    CountryType *country;
    unsigned int age;
} CitizenType;

typedef struct {
    CitizenType *citizen;
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
} MonitorData;

typedef struct {
    HashTable *countries;
    HashTable *viruses;
    unsigned int bloom_size;
    unsigned int exp_records;
} TM_Data;

/* ------------------------------------ Basic Operations ---------------------------------------- */

/* VaccinationType */
void vac_destroy(VaccinationType **vaccination);

/* CountryType */
void cnt_destroy(CountryType **country);

/* CitizenType */
void ctz_destroy(CitizenType **citizen);

/* VirusInfo */
VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records);

void vir_destroy(VirusInfo **virus);

/* MonitorData */
MonitorData *gdt_create(unsigned int bloom_size, unsigned int exp_records);

void gdt_destroy(MonitorData **gen_data);

/* TravelMonitor Data */
TM_Data *mtr_create(unsigned int bloom_size, unsigned int exp_records);

void mtr_destroy(TM_Data **mon_data);


#endif //SYSPRO_HW1_DATA_H
