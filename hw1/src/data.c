/* data.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    11-04-2021
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/data.h"


/* -------------------------- Basic Operations ------------------------------ */

/* VaccinationType */
void vac_destroy(VaccinationType **vaccination) {
    free((*vaccination)->date);
    free(*vaccination);
    *vaccination = NULL;
}

/* CountryType */
void cnt_destroy(CountryType **country) {
    free((*country)->name);
    free(*country);
    *country = NULL;
}

/* CitizenType */
void ctz_destroy(CitizenType **citizen) {
    free((*citizen)->first_name);
    free((*citizen)->last_name);
    free(*citizen);
    *citizen = NULL;
}

/* VirusInfo */
VirusInfo *vir_create(char *name, unsigned int bloom_size, unsigned int exp_records) {
    VirusInfo *v_info = (VirusInfo *) malloc(sizeof(VirusInfo));
    v_info->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(v_info->name, name);
    v_info->filter = blf_create(exp_records, bloom_size);
    v_info->vaccinated = skl_create();
    v_info->not_vaccinated = skl_create();
    v_info->vaccinations_by_date = skl_create();

    return v_info;
}

void vir_destroy(VirusInfo **virus) {
    free((*virus)->name);
    blf_destroy(&(*virus)->filter);
    skl_destroy_all(&(*virus)->vaccinated, (FP_item_free) vac_destroy);
    skl_destroy(&(*virus)->not_vaccinated);
    skl_destroy(&(*virus)->vaccinations_by_date);
    free(*virus);
    *virus = NULL;
}

/* GeneralData */
GeneralData *gdt_create(unsigned int bloom_size, unsigned int exp_records) {
    GeneralData *gen_data = (GeneralData *) malloc(sizeof(GeneralData));
    gen_data->bloom_size = bloom_size;
    gen_data->exp_records = exp_records;
    gen_data->citizens = htb_create(exp_records);
    gen_data->countries = htb_create(200);
    gen_data->viruses = htb_create(1000);

    return gen_data;
}

void gdt_destroy(GeneralData **gen_data) {
    htb_destroy_all(&(*gen_data)->citizens, (FP_item_free) ctz_destroy);
    htb_destroy_all(&(*gen_data)->countries, (FP_item_free) cnt_destroy);
    htb_destroy_all(&(*gen_data)->viruses, (FP_item_free) vir_destroy);

    free(*gen_data);
    *gen_data = NULL;
}