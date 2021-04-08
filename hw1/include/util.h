/* util.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_UTIL_H
#define SYSPRO_HW1_UTIL_H

#include "bloomfilter.h"
#include "skiplist.h"

#define MAGIC_SIZE 200



typedef struct {
    char *name;
    BloomFilter *filter;
    SkipList *vaccinated_list;
    SkipList *not_vaccinated_list;
} VirusInfo;

typedef struct {
    VirusInfo *viruses[MAGIC_SIZE];
    CountryType *countries[MAGIC_SIZE];
    CitizenType *citizens[MAGIC_SIZE];
    unsigned int bloom_size;
    unsigned int exp_records;
} GeneralData;


VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records);

void vir_insert(VirusInfo *virus, CitizenType *citizen, int vaccinated);

void vir_vaccinate(VirusInfo *virus, CitizenType *citizen);

void vir_destroy(VirusInfo **virus);

int record_parser(char *rec_file_name, GeneralData *data);

int input_handler(char *input);


#endif //SYSPRO_HW1_UTIL_H
