/* handler.h
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#ifndef SYSPRO_HW1_HANDLER_H
#define SYSPRO_HW1_HANDLER_H

enum cmd_code {
    EXIT,
    VACCINE_STATUS_BLOOM,
    VACCINE_STATUS,
    POPULATION_STATUS,
    POP_STATUS_BY_AGE,
    INSERT_CITIZEN_RECORD,
    VACCINATE_NOW,
    LIST_NON_VACCINATED
};

void handler(GeneralData *data);

#endif //SYSPRO_HW1_HANDLER_H
