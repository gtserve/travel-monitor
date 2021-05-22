/* comm_protocol.h
* -------------------------------------------------------------------------------------------------
* course:   System Programming
* project:  (#2) Travel Monitor
* name:     George Tservenis
* email:    sdi1500164@di.uoa.gr
* -------------------------------------------------------------------------------------------------
*/

#ifndef SYSPRO_HW2_COMM_PROTOCOL_H
#define SYSPRO_HW2_COMM_PROTOCOL_H

#define SEP_TOKEN "#"

typedef enum {CDIR, BFILTER, CDIRS_DONE} OP_CODE;

int encode_str(OP_CODE code, char *str, char **message);

OP_CODE decode_op(char **message);


#endif //SYSPRO_HW2_COMM_PROTOCOL_H
