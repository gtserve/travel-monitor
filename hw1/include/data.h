/* data.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_DATA_H
#define SYSPRO_HW1_DATA_H

#include "hashtable.h"
#include "bloomfilter.h"
#include "skiplist.h"


/* ---------------------------- Data Types ---------------------------------- */

typedef struct {
    char *name;
    unsigned int population;
} CountryType;

typedef struct {
    int id;
    char *first_name;
    char *last_name;
    CountryType *country;
    unsigned int age;
//    HashTable *vaccinations;
} CitizenType;

typedef struct {
    CitizenType *citizen;
    char *date;
} VaccinationType;

typedef struct {
    char *name;
    BloomFilter *filter;
    SkipList *vaccinated_list;
    SkipList *not_vaccinated_list;
} VirusInfo;

typedef struct {
    HashTable *citizens;
    HashTable *countries;
    HashTable *viruses;
    unsigned int bloom_size;        // Bloom Filter size.
    unsigned int exp_records;       // Expected records.
} GeneralData;

/* -------------------------- Basic Operations ------------------------------ */

/* VaccinationType */
void vac_destroy(VaccinationType **vaccination);

/* CitizenType */
void cnt_destroy(CountryType **country);

/* CitizenType */
void ctz_destroy(CitizenType **citizen);

/* VirusInfo */
VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records);

void vir_insert(VirusInfo *virus, CitizenType *citizen, int vaccinated);

void vir_vaccinate(VirusInfo *virus, CitizenType *citizen);

void vir_destroy(VirusInfo **virus);

/* GeneralData */
GeneralData *gdt_create(unsigned int bloom_size, unsigned int exp_records);

void gdt_destroy(GeneralData **gen_data);





#endif //SYSPRO_HW1_DATA_H
