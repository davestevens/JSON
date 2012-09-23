#include "json.h"

/* Loop through json_t structure concatenate data onto provided string */
char *json_encode_internal(json_t *j, char *s)
{
	int i;
	switch(j->type) {
		case OBJECT:
			strncat(s, "{", 1);
			for(i=0;i<(int)j->num;++i) {
				if(j->content) {
					json_encode_internal(j->content + i, s);
				}
				if(i < (int)j->num - 1) {
					strncat(s, ",", 1);
				}
			}
			strncat(s, "}", 1);
			break;
		case ARRAY:
			strncat(s, "[", 1);
			for(i=0;i<(int)j->num;++i) {
				if(j->content) {
					json_encode_internal(j->content + i, s);
				}
				if(i < (int)j->num - 1) {
					strncat(s, ",", 1);
				}
			}
			strncat(s, "]", 1);
			break;
		case NUMBER:
		{
			char str[256] = {'\0'};
			switch(j->value.num.type) {
				case INTEGER:
					sprintf(str, "%d", j->value.num.value.ival);
					break;
				case FLOAT:
					sprintf(str, "%f", j->value.num.value.fval);
					break;
				case REAL:
					sprintf(str, "%g", j->value.num.value.dval);
					break;
				default:
					break;				
			}
			strncat(s, str, strlen(str));
		}
			break;
		case STRING:
			if(j->value.str == NULL) {
				strncat(s, "null", 4);
			}
			else {
				strncat(s, "\"", 1);
				strncat(s, j->value.str, strlen(j->value.str));
				strncat(s, "\"", 1);
			}
			break;
		case BOOL:
			if(j->value.bool) {
				strncat(s, "true", 4);
			}
			else {
				strncat(s, "false", 5);
			}
			break;
		case PAIR:
			strncat(s, j->name, strlen(j->name));
			strncat(s, ":", 1);
			json_encode_internal(j->content, s);
			break;
		case ROOT:
			json_encode_internal(j->content, s);
			break;
		default:
			fprintf(error, "UNKNONW\n");
			break;
			
	}
	return (char *)s;
}
