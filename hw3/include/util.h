/* util.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW1_UTIL_H
#define SYSPRO_HW1_UTIL_H

// Error messages
#define ERR_MSG_FOPEN       "ERROR: Couldn't open file '%s'.\n"
#define ERR_MSG_NOT_INT     "ERROR: Argument %s must be an integer.\n"
#define ERR_MSG_NOT_POS     "ERROR: Argument %s must be positive.\n"
#define ERR_MSG_ARG         "ERROR: Option -%c requires an argument.\n"
#define ERR_MSG_UNKNOWN     "ERROR: Unknown option -%c.\n"
#define ERR_MSG_NOT_PRINT   "ERROR: Unknown option character '\\x%x'.\n"
#define ERR_MSG_INV_DIR     "ERROR: Invalid name '%s' for directory.\n"
#define ERR_MSG_DIR_NEXST   "ERROR: Directory '%s' doesn't exist.\n"

// Error Operations
#define ERR_CHECK_NULL_RETURN(ptr, msg, ret)        \
{                                                   \
    if (!(ptr)) {                                   \
        fprintf(stderr, (msg));                     \
        return (ret);                               \
    }                                               \
}

#define ERR_CHECK_NULL(ptr, msg)                \
{                                               \
    if (!(ptr)) {                               \
        fprintf(stderr, (msg));                 \
        return;                                 \
    }                                           \
}

#define ERR_CHECK_ARG(check, msg, arg)          \
{                                               \
    if (check) {                                \
        fprintf(stderr, (msg), (arg));          \
        exit(EXIT_FAILURE);                     \
    }                                           \
}

#define ERR_CHECK(check, msg)                   \
{                                               \
    if (check) {                                \
        fprintf(stderr, (msg));                 \
        exit(EXIT_FAILURE);                     \
    }                                           \
}

#define PATH_SIZE 256
//#define SEP_TOKEN "$"
#define TOK_DELIM " \n"

// String Operations
#define STR_EQUALS(str1, str2) (strcmp((str1), (str2)) == 0)
#define STR_BYTES(str) (strlen(str) + 1)
#define STR_CPY(src, dest)                                          \
{                                                                   \
    dest = (char *) malloc(sizeof(char) * STR_BYTES(src));          \
    strcpy((dest), (src));                                          \
}


typedef void (* FP_item_free) (void **);


int str_is_alpha(char *str);

int str_is_int(char *str);

int str_is_alnum(char *str);

int str_is_alpha_or_c(char *str, char c);

int str_is_alphanum_or_c(char *str, char c);

int str_is_digit_or_c(char *str, char c);

void str_replace(char *str, int length, char a, char b);

char *get_todays_date(void);

int date_to_days(char *date);

int compose_key(int id, char *date);

int get_age_group(int age);


#endif //SYSPRO_HW1_UTIL_H
