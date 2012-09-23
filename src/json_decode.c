#include "json.h"

/*
 */
unsigned int json_decode_array(json_t *parent, char *p)
{
	char *f = p + 1;
	json_t *j = connect(parent);
	j->type = ARRAY;

	while(*f != ']') {
		f += skip_whitespace(f);

		switch((char)*f) {
			case '[':
				f += json_decode_array(j, f);
				break;
			case '{':
				/* Object */
				f += json_decode_object(j, f);
				break;
			case '"':
				f += json_decode_string(j, f);
				break;
			case 't':
			case 'f':
				f += json_decode_bool(j, f);
				break;
			case 'n':
				f += json_decode_null(j, f);
				break;
			case ',': /* Delimeter */
				++f;
				break;
			default:
				f += json_decode_number(j, f);
				break;
		}

		f += skip_whitespace(f);
	}
	return (unsigned int)((f - p) + 1);
}

/*
 */
unsigned int json_decode_object(json_t *parent, char *p)
{
	char *f = p + 1;
	json_t *j = connect(parent);
	j->type = OBJECT;

	while(*f != '}') {
		f += skip_whitespace(f);
		switch((char)*f) {
			case ',': /* Delimeter */
				++f;
				break;
			default:
				f += json_decode_pair(j, f);
				break;
		}
		f += skip_whitespace(f);
	}
	return (unsigned int)((f - p) + 1);
}

/*
 */
unsigned int json_decode_pair(json_t *parent, char *p)
{
	/* Get string : value */
	char *f = p;
	json_t *j = connect(parent);
	j->type = PAIR;

	/* TODO: error checking that a valid pair is presented */
	while((*f != ':') && (*f != ' ') && (*f != '\t')) {
		++f;
	}

	if(*p == '"') {
		char *n = (char *)calloc((f - p) - 1, sizeof(char));
		strncpy(n, (p + 1), (f - (p + 2)));
		j->name = n;
	}
	else {
		char *n = (char *)calloc((f - p) + 1, sizeof(char));
		strncpy(n, p, (f - p));
		j->name = n;
	}

	f += skip_whitespace(f);
	/* Should then be ':' char */
	if((char)*f == ':') {
		++f;
	}
	else {
		fprintf(error, "Object error, expecting ':' got '%c'\n", (char)*f);
	}
	f += skip_whitespace(f);

	/* figure type of element */
	switch((char)*f) {
		case '[':
			f += json_decode_array(j, f);
			break;
		case '{':
			f += json_decode_object(j, f);
			break;
		case ',': /* Delimiter */
			++f;
			break;
		case 't':
		case 'f':
			f += json_decode_bool(j, f);
			break;
		case 'n':
			f += json_decode_null(j, f);
			break;
		case '"':
			f += json_decode_string(j, f);
			break;
		default:
			f += json_decode_number(j, f);
			break;
	}

	f += skip_whitespace(f);

	return (unsigned int)(f - p);
}

/*
  Number
  integer, float or double
  generate a new json_t, include data and connect to parent json_t
 */
unsigned int json_decode_number(json_t *parent, char *p)
{
	char *f = p;
	json_t *j = connect(parent);
	j->type = NUMBER;

	while((*f != ',') && (*f != ']') && (*f != '}')) {
		++f;
	}

	{
		unsigned char type = INTEGER;
		number_t *n = (number_t *)&(j->value.num);
		char *i = (char *)calloc((f - p) + 1, sizeof(char));
		strncpy(i, p, (f - p));

		/* Check for type: integer, real or float */
		if(strchr(i, (int)'.') != NULL) {
			type = FLOAT;
		}
		if((strchr(i, (int)'e') || strchr(i, (int)'E'))) {
			type = REAL;
		}

		/* Dependent on the type create a new instance of number */
		switch(type) {
			case INTEGER:
			{
				/* Convert from string to int */
				n->value.ival = atoi(i);
				n->type = INTEGER;
			}
			break;
			case FLOAT:
			{
				/* Convert from string to float */
				sscanf(i, "%f", &(n->value.fval));
				n->type = FLOAT;
			}
			break;
			case REAL:
			{
				sscanf(i, "%lf", &(n->value.dval));
				n->type = REAL;
			}
			break;
			default:
			{
				fprintf(error, "Unknown number type (%d)\n", type);
			}
			break;
		}
		free(i);
	}
	return (unsigned int)(f - p);
}

/*
 */
unsigned int json_decode_string(json_t *parent,char *p)
{
	unsigned char escaped = 0;
	char *f = (p + 1);
	json_t *j = connect(parent);
	j->type = STRING;

	/* Loop through string, find end " */
	/* Take into account escaped " */
	while((*f != '"') || escaped) {
		if(escaped) { escaped = 0; }
		if(*f == '\\') { escaped = 1; }
		++f;
	}
	f += 1;

	{
		/* Ignore leading and trailing " characters */
		char *s = (char *)calloc((f - p) - 1, sizeof(char));
		strncpy(s, (p + 1), (f - (p + 2)));
		j->value.str = s;
	}

	return (unsigned int)(f - p);
}

/*
 */
unsigned int json_decode_null(json_t *parent, char *p)
{
	char *f = p;
	json_t *j = connect(parent);
	j->type = STRING;

	while((*f != ',') && (*f != ']') && (*f != '}')) {
		++f;
	}

	if(!strncmp(p, "null", 4)) {
		j->value.str = NULL;
	}
	else {
		fprintf(error, "Unknown NULL type (%s)\n", p);
		exit(-1);
	}
	return (unsigned int)(f - p);
}

/*
  Bool
  true or false
 */
unsigned int json_decode_bool(json_t *parent, char *p)
{
	char *f = p;
	json_t *j = connect(parent);
	j->type = BOOL;

	while((*f != ',') && (*f != ']') && (*f != '}')) {
		++f;
	}

	if(!strncmp(p, "true", 4)) {
		j->value.bool = 1;
	}
	else if(!strncmp(p, "false", 5)) {
		j->value.bool = 0;
	}
	else {
		fprintf(error, "Unknown BOOL type (%s)\n", p);
		exit(-1);
	}
	return (unsigned int)(f - p);
}

/*
  Connect j to parent.
  if content is null this is the first item otherwise it is part of a list
 */
json_t *connect(json_t *parent)
{
	if(parent->content == NULL) {
		parent->content = (json_t *)calloc(1, sizeof(json_t));
		if(parent->content == NULL) {
			fprintf(error, "Memory allocation error (calloc)\n");
			exit(-1);
		}
		++parent->num;
		return (json_t *)parent->content;
	}
	else {
		++parent->num;
		parent->content = (json_t *)realloc(parent->content, sizeof(json_t) * parent->num);
		if(parent->content == NULL) {
			fprintf(error, "Memory allocation error (realloc)\n");
			exit(-1);
		}
		{
			json_t *temp = (json_t *)(parent->content + (parent->num - 1));
			temp->content = NULL;
			temp->num = 0;
		}
		return (json_t *)(parent->content + (parent->num - 1));
	}
	return (json_t *)NULL;
}

unsigned int skip_whitespace(char *p)
{
	char *f = p;
	while((*f == ' ') || (*f == '\t') || (*f == '\n')) {
		++f;
	}
	return (unsigned int)(f - p);
}
