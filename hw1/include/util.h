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

#include "data.h"

#define TOK_DELIM " \n"

#define STR_EQUALS(str1, str2) (strcmp((str1), (str2)) == 0)
#define STR_BYTES(str) (strlen(str) + 1)
#define STR_CPY(src, dest)                                          \
{                                                                   \
    dest = (char *) malloc(sizeof(char) * STR_BYTES(src));          \
    strcpy((dest), (src));                                          \
}

int str_is_alpha(char *str);

int str_is_int(char *str);

int str_is_alpha_or_c(char *str, char c);

int str_is_digit_or_c(char *str, char c);

void str_replace(char *str, int length, char a, char b);

char *get_todays_date(void);

int record_parser(char *rec_file_name, GeneralData *gen_data);


#endif //SYSPRO_HW1_UTIL_H
