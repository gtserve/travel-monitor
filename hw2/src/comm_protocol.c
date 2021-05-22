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

#include "../include/monitor.h"
#include "../include/comm_protocol.h"

// OP_CODE#George#


int encode_str(OP_CODE code, char *str, char **payload) {

    int str_size = (int) strlen(str);
    int msg_bytes = (int) sizeof(OP_CODE) + 1 + str_size + 1;

    *payload = (char *) malloc(msg_bytes + 1);

    sprintf(*payload, "%d%s%s%s", code, SEP_TOKEN, str, SEP_TOKEN);

    return msg_bytes;
}

OP_CODE decode_op(char **payload) {
    // TODO: moves pointer

    // Get Op. Code
    char *op_str = strtok(*payload, SEP_TOKEN);
    OP_CODE op_code = strtol(op_str, NULL, 10);

    *payload = strtok(NULL, SEP_TOKEN);
    return op_code;
}
