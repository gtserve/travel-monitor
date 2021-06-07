/* comm_protocol.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#3) Travel Monitor (sockets, threads)
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW2_COMM_PROTOCOL_H
#define SYSPRO_HW2_COMM_PROTOCOL_H

#define SEND_STR(code, str, fd, buf_size) {                 \
    char *payload = NULL;                                   \
    int msg_bytes = encode_str((code), (str), (&payload));  \
    msg_send((fd), (buf_size), (payload), (msg_bytes));     \
    free(payload);                                          \
}

#define SEND_DATA(code, data, data_bytes, fd, buf_size) {                       \
    char *payload = NULL;                                                       \
    int msg_bytes = encode_data((code), (data), (data_bytes), (&payload));      \
    msg_send((fd), (buf_size), (payload), (msg_bytes));                         \
    free(payload);                                                              \
}

#define RECEIVE(code, payload, fd, buf_size, del_ptr) {     \
    msg_get((fd), (buf_size), &(payload));                  \
    (del_ptr) = (payload);                                  \
    (code) = decode_op(&(payload));                         \
}


typedef enum {
    CDIR,
    CDIRS_DONE,
    BFILTER,
    BFILTERS_DONE,
    SL_QUERY,
    ID_QUERY,
    YES,
    NO,
    UNKNOWN,
    EXIT
} OP_CODE;

int encode_str(OP_CODE code, char *str, char **payload);

int encode_data(OP_CODE code, char *data, int data_bytes, char **payload);

OP_CODE decode_op(char **payload);


#endif //SYSPRO_HW2_COMM_PROTOCOL_H
