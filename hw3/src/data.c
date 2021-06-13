/* data.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../include/data.h"

#define EXP_COUNTRIES 200
#define EXP_VIRUSES 1000


/* ------------------------------------ Basic Operations ---------------------------------------- */

/* TravelRequest */
void trq_destroy(TravelRequest **travel_req) {
    free((*travel_req)->date);
    free((*travel_req)->country_name);
    free(*travel_req);
    *travel_req = NULL;
}

/* VaccinationType */
void vac_destroy(VaccinationType **vaccination) {
    free((*vaccination)->date);
    free(*vaccination);
    *vaccination = NULL;
}

/* Country */
void cnt_destroy(Country **country) {
    free((*country)->name);
    free(*country);
    *country = NULL;
}

/* Citizen */
void ctz_destroy(Citizen **citizen) {
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

/* MonitorData */
MonitorData *mnd_create(unsigned int bloom_size, unsigned int exp_records) {
    MonitorData *m_data = (MonitorData *) malloc(sizeof(MonitorData));

    pthread_mutex_init(&(m_data->lock), NULL);
    m_data->bloom_size = bloom_size;
    m_data->exp_records = exp_records;

    m_data->num_req_total = 0;
    m_data->num_req_accepted = 0;
    m_data->num_req_rejected = 0;

    m_data->citizens = htb_create(exp_records);
    m_data->parsed_files = htb_create(EXP_COUNTRIES);
    m_data->countries = htb_create(EXP_COUNTRIES);
    m_data->viruses = htb_create(EXP_VIRUSES);

    return m_data;
}

void mnd_destroy(MonitorData **gen_data) {
    pthread_mutex_destroy(&((*gen_data)->lock));

    htb_destroy_all(&(*gen_data)->citizens, (FP_item_free) ctz_destroy);
    htb_destroy(&(*gen_data)->parsed_files, 1);
    htb_destroy_all(&(*gen_data)->countries, (FP_item_free) cnt_destroy);
    htb_destroy_all(&(*gen_data)->viruses, (FP_item_free) vir_destroy);

    free(*gen_data);
    *gen_data = NULL;
}

TM_MonitorData *tmm_create(unsigned int bloom_size, unsigned int exp_records) {
    TM_MonitorData *tmm_data = (TM_MonitorData *) malloc(sizeof(TM_MonitorData));
    tmm_data->bloom_size = bloom_size;
    tmm_data->exp_records = exp_records;
    tmm_data->countries = htb_create(EXP_COUNTRIES);
    tmm_data->viruses = htb_create(EXP_VIRUSES);

    return tmm_data;
}

void tmm_destroy(TM_MonitorData **tmm_data) {
    htb_destroy_all(&(*tmm_data)->countries, (FP_item_free) cnt_destroy);
    htb_destroy_all(&(*tmm_data)->viruses, (FP_item_free) vir_destroy);

    free(*tmm_data);
    *tmm_data = NULL;
}


/* TM_Data */
TM_Data *tmd_create(int num_monitors, unsigned int bloom_size, unsigned int exp_records) {
    TM_Data *tm_data = (TM_Data *) malloc(sizeof(TM_Data));

    tm_data->num_monitors = num_monitors;
    tm_data->num_req_total = 0;
    tm_data->num_req_accepted = 0;
    tm_data->num_req_rejected = 0;

    tm_data->channels = (SocketChannel *) malloc(sizeof(SocketChannel) * num_monitors);

    tm_data->mon_data = (TM_MonitorData **) malloc(sizeof(TM_MonitorData) * num_monitors);
    for (int i = 0; i < num_monitors; i++)
        tm_data->mon_data[i] = tmm_create(bloom_size, exp_records);

    tm_data->country_to_monitor = htb_create(EXP_COUNTRIES);
    tm_data->virus_to_requests = htb_create(EXP_COUNTRIES);
    tm_data->all_countries = htb_create(EXP_COUNTRIES);

    return tm_data;
}

void tmd_destroy(TM_Data **tm_data) {
    for (int i = 0; i < (*tm_data)->num_monitors; i++)
        tmm_destroy(&((*tm_data)->mon_data[i]));
    free((*tm_data)->mon_data);

    free((*tm_data)->channels);

    htb_destroy(&(*tm_data)->country_to_monitor, 1);
    htb_destroy_all(&(*tm_data)->virus_to_requests, (FP_item_free) skl_destroy_req);
    htb_destroy(&(*tm_data)->all_countries, 0);

    free(*tm_data);
    *tm_data = NULL;
}