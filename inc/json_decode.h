#ifndef __GUARD_DECODE_JSON_H
#define __GUARD_DECODE_JSON_H

/* Internal functions */
unsigned int json_decode_array(json_t *, char *);
unsigned int json_decode_object(json_t *, char *); /* json_t */
unsigned int json_decode_pair(json_t *, char *);

unsigned int json_decode_number(json_t *, char *); /* Union (integer, float, double) */
unsigned int json_decode_string(json_t *, char *); /* char * */
unsigned int json_decode_null(json_t *, char *); /* char * */
unsigned int json_decode_bool(json_t *, char *); /* True or False */ /* unsigned int */

unsigned int skip_whitespace(char *);
json_t *connect(json_t *);

#endif
