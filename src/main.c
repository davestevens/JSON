#include <stdio.h>

#include "json.h"

/*
  Test of the json_decode, json_print, json_encode and json_free functions
 */
int main(void)
{
	/* Test 1 */
	{
		char *str = "{\"foo\":\"bar\", array:[0,1,2,3], object: {name: 1.23456}}";
		json_t *test = json_decode(str);

		/* Print structure */
		json_print(test);

		/* Encode the structure back into json string */
		printf("\njson_encode: %s\n\n", json_encode(test));

		/* Clean up */
		json_free(test);
	}

	/* Test 2 */
	{
		char *str = "[34e-9, 35e-9, 36e-9, {obj: [21, 22, 23]}, \"this is a string\", true, false, null]";
		json_t *test = json_decode(str);

		/* Print structure */
		json_print(test);

		/* Modify data */

		/* multiply Real numbers by 100 */
		{
			int i;
			for(i=0;i<(int)test->content[0].num;++i) {
				switch(test->content[0].content[i].type) {
					case NUMBER:
					{
						json_t *t = &(test->content[0].content[i]);
						switch(t->value.num.type) {
							case INTEGER:
								t->value.num.value.ival *= 100;
								break;
							case FLOAT:
								t->value.num.value.fval *= 100;
								break;
							case REAL:
								t->value.num.value.dval *= 100;
								break;
							default:
								break;
						}
					}
					default:
						break;
				}
			}
		}

		/* Multiple numbers in obj by 5 */
		{
			int i;
			json_t *t = &(test->content[0].content[3].content[0]);
			printf("Object name: %s\n", t->name);
			for(i=0;i<(int)t->content[0].num;++i) {
				switch(t->content[0].content[i].type) {
					case NUMBER:
					{
						json_t *_t = &(t->content[0].content[i]);
						switch(_t->value.num.type) {
							case INTEGER:
								_t->value.num.value.ival *= 5;
								break;
							case FLOAT:
								_t->value.num.value.fval *= 5;
								break;
							case REAL:
								_t->value.num.value.dval *= 5;
								break;
							default:
								break;
						}
					}
					default:
						break;
						
				}
			}
		}

		/* Encode the structure back into json string */
		printf("\njson_encode: %s\n\n", json_encode(test));

		/* Clean up */
		json_free(test);
	}
	return 0;
}
