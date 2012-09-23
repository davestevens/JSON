#include "json.h"

char *json_type(unsigned int);
void json_print_internal(json_t *, unsigned int);

/*
 */
json_t *json_decode(char *str)
{
	/* Create an instance of json_t */
	/* Loop though string
	   Figure out type of object
	*/
	char *p = str;
	json_t *j = (json_t *)calloc(1, sizeof(json_t));
	if(j == NULL) {
		fprintf(error, "Memory allocation error\n");
		exit(-1);
	}
	j->type = ROOT;

	while(p < (str + strlen(str))) {
		p += skip_whitespace(p);

		switch((char)*p) {
			case '[':
				p += json_decode_array(j, p);
				break;
			case '{':
				p += json_decode_object(j, p);
				break;
			case '"':
				p += json_decode_string(j, p);
				break;
			case 't':
			case 'f':
				p += json_decode_bool(j, p);
				break;
			case 'n':
				p += json_decode_null(j, p);
				break;
			default:
				p += json_decode_number(j, p);
				break;
		}
		p += skip_whitespace(p);
	}
	return (json_t *)j;
}

/*
  Allocate memory for string and call internal json encode function
*/
char *json_encode(json_t *j)
{
	char *s;
	if(j == NULL) {
		return (char *)NULL;
	}
	s = (char *)calloc(2048, sizeof(char));
	return json_encode_internal(j, s);
}

/* TODO:
   fixme
*/
int json_free(json_t *j)
{
	int i;
	if(j == NULL) {
		return -1;
	}

	switch(j->type) {
		case STRING:
			if(j->value.str != NULL) {
				free(j->value.str);
			}
			break;
		case PAIR:
			free(j->name);
			break;
		default:
			break;
	}

	for(i=0;i<(int)j->num;++i) {
		if(j->content) {
			json_free(j->content + i);
		}
	}
	if((j->content != NULL) && (j->num > 0)) {
		free(j->content);
	}
	if(j->type == ROOT) {
		free(j);
	}
	return 0;
}

/*
 */
void json_print(json_t *j)
{
	json_print_internal(j, 0);
	return;
}

/*
 */
void json_print_internal(json_t *j, unsigned int indent)
{
	int i;
	if(indent == 0) {
		printf("JSON (%p)\n", (void *)j);
	}
	if(j == NULL) {
		return;
	}
	/* Loop through json_t object */
	for(i=0;i<(int)indent;++i) { printf("\t"); }
	printf("%s: ", json_type(j->type));

	switch(j->type) {
		case NUMBER:
			switch(j->value.num.type) {
				case INTEGER:
					printf("%d", j->value.num.value.ival);
					break;
				case FLOAT:
					printf("%f", j->value.num.value.fval);
					break;
				case REAL:
					printf("%g", j->value.num.value.dval);
					break;
				default:
					break;
			}
			break;
		case STRING:
			printf("%s", j->value.str);
			break;
		case BOOL:
			printf("%s", j->value.bool ? "true" : "false");
			break;
		case PAIR:
			printf("%s", j->name);
			break;
		default:
			break;
	}

	printf("\n");
	for(i=0;i<(int)j->num;++i) {
		if(j->content) {
			json_print_internal((j->content + i), ++indent);
			--indent;
		}
	}
	return;
}

/*
 */
char *json_type(unsigned int type)
{
	switch(type) {
		case ROOT:
			return "ROOT";
			break;
		case ARRAY:
			return "ARRAY";
			break;
		case OBJECT:
			return "OBJECT";
			break;
		case NUMBER:
			return "NUMBER";
			break;
		case STRING:
			return "STRING";
			break;
		case BOOL:
			return "BOOL";
			break;
		case PAIR:
			return "PAIR";
			break;
		default:
			return "Unknown";
			break;
	}
}

