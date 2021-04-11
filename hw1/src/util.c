/* util.c
 * ----------------------------------------------------------------------------
 * System Programming - Homework 1
 * name:    George Tservenis
 * email:   sdi1500164@di.uoa.gr
 * date:    25-03-2021
 * ----------------------------------------------------------------------------
 */

#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/util.h"


void str_replace(char *str, int length, char a, char b) {
    for (int i = 0; i < length; i++) {
        if (str[i] == a)
            str[i] = b;
    }
}

int str_is_alpha(char *str) {
    while ((*str) && isalpha(*(str++)));
    return ((*str) ? 0 : 1);
}

int str_is_int(char *str) {
    while ((*str) && isdigit(*(str++)));
    return ((*str) ? 0 : 1);
}

int str_is_alpha_or_c(char *str, char c) {
    while ((*str) && (isalpha(*str) || (*str == c))) {
        str++;
    }
    return ((*str) ? 0 : 1);
}

int str_is_digit_or_c(char *str, char c) {
    while ((*str) && (isdigit(*str) || (*str == c))) {
        str++;
    }
    return ((*str) ? 0 : 1);
}

char *get_todays_date(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *date = (char *) malloc(sizeof(char) * 11);
    sprintf(date, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1,
            tm.tm_year + 1900);
    return date;
}
