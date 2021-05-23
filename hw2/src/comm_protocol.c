/* comm_protocol.c
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "../include/comm_protocol.h"

// OP_CODE#George#


int encode_str(OP_CODE code, char *str, char **payload) {

    int str_size = (int) strlen(str);
    int msg_bytes = (int) sizeof(OP_CODE) + str_size + 1;

    int offset = 0;
    *payload = (char *) malloc(msg_bytes);

    memcpy((*payload) + offset, &code, sizeof(OP_CODE));
    offset += sizeof(OP_CODE);

    memcpy((*payload) + offset, str, (str_size + 1));

    return msg_bytes;
}

int encode_data(OP_CODE code, char *data, int data_bytes, char **payload) {

    int msg_bytes = (int) sizeof(OP_CODE) + data_bytes;

    int offset = 0;
    *payload = (char *) malloc(msg_bytes);

    memcpy((*payload) + offset, &code, sizeof(OP_CODE));
    offset += sizeof(OP_CODE);

    memcpy((*payload) + offset, data, data_bytes);

    return msg_bytes;
}

OP_CODE decode_op(char **payload) {

    // Get Op. Code
    OP_CODE op_code;
    memcpy(&op_code, (*payload), sizeof(OP_CODE));

    (*payload) += sizeof(OP_CODE);

    return op_code;
}
