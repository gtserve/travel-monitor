/* util.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "../include/util.h"

#define FIRST_DATE "01-01-1600"

#define EVAL_DIGIT(c) ((c) - '0')

static int fd_days = -1;

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

int str_is_alnum(char *str) {
    while ((*str) && isalnum(*(str++)));
    return ((*str) ? 0 : 1);
}

int str_is_alpha_or_c(char *str, char c) {
    while ((*str) && (isalpha(*str) || (*str == c))) {
        str++;
    }
    return ((*str) ? 0 : 1);
}

int str_is_alphanum_or_c(char *str, char c) {
    while ((*str) && (isalnum(*str) || (*str == c))) {
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
    sprintf(date, "%02d-%02d-%d", (unsigned int) tm.tm_mday % 31,
            (unsigned int) (tm.tm_mon + 1) % 13,
            (unsigned int) (tm.tm_year + 1900) % 10000);
    return date;
}

int date_to_days(char *date) {
    // DD-MM-YYYY
    // 01-34-6789

    int d = EVAL_DIGIT(date[0]) * 10 + EVAL_DIGIT(date[1]);
    int m = EVAL_DIGIT(date[3]) * 10 + EVAL_DIGIT(date[4]);
    int y = EVAL_DIGIT(date[6]) * 1000 + EVAL_DIGIT(date[7]) * 100 +
            EVAL_DIGIT(date[8]) * 10 + EVAL_DIGIT(date[9]);

    return (y - 1) * 360 + (m - 1) * 30 + (d - 1);
}

int compose_key(int id, char *date) {
    if (fd_days == -1)
        fd_days = date_to_days(FIRST_DATE);
    int days_offset = date_to_days(date) - fd_days;

    return days_offset * 10000 + id;
}

int get_age_group(int age) {
    if (age < 20) return 0;
    if (age < 40) return 1;
    if (age < 60) return 2;
    return 3;
}

/* File operations */

int count_files(char *dir_name) {
    int num_files = 0;

    DIR* dir = NULL;
    if ((dir = opendir(dir_name)) == NULL) {
        perror(dir_name);
        exit(-1);
    }

    struct dirent *dent = NULL;
    for (int j = 0; ((dent = readdir(dir)) != NULL); j++) {
        if (dent->d_type == DT_REG)
            num_files++;
    }

    return num_files;
}

