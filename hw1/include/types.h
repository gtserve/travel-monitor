#ifndef SYSPRO_HW1_TYPES_H
#define SYSPRO_HW1_TYPES_H

typedef struct {
    char *name;
    int population;
} CountryType;

typedef struct {
    int id;
    char *firstName;
    char *lastName;
    CountryType *country;
    int age;
} CitizenType;

#endif //SYSPRO_HW1_TYPES_H
