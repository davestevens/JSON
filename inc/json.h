#ifndef __GUARD_JSON_H
#define __GUARD_JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implementation of json structure
   Defined at: http://www.json.org/
*/

#define error stderr

/* Types of possible numbers */
typedef enum number_e {
	INTEGER, FLOAT, REAL
} number_e;

/* Types of json_t element */
typedef enum json_e {
	ROOT, ARRAY, OBJECT, NUMBER, STRING, BOOL, PAIR
} json_e;

/* Number element structure */
typedef struct number_t {
	number_e type;			/* Identier */
	union {
		int ival;		/* Integer */
		float fval;		/* Float */
		double dval;		/* Real */
	} value;
} number_t;

/* JSON element structure */
typedef struct json_t {
	json_e type;			/* Identifier */
	unsigned int num;		/* Size of content */
	char *name;			/* Name, used by object */
	union {
		number_t num; 		/* Number */
		char *str;		/* String & Null */
		unsigned char bool;	/* Bool */
	} value;
	struct json_t *content;
} json_t;

/* External functions */
json_t *json_decode(char *);
char *json_encode(json_t *);
int json_free(json_t *);
void json_print(json_t *);

/* Internal functionality */
#include "json_decode.h"
#include "json_encode.h"

#endif
