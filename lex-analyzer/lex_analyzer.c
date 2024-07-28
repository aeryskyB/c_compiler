#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lex.h"

#define TOKEN_TYPE_MAX_LEN 20
#define TOKEN_VAL_MAX_LEN 30

typedef struct {
	char Type[TOKEN_TYPE_MAX_LEN];
	char Value[TOKEN_VAL_MAX_LEN];
} Token;

File *input;
// char *text;

int main(int argc, char **argv) {

	assert(argc == 2);

	input = fopen(argv[1], "r");
	if (input == NULL) {
		fprintf(stderr, "Error opening file: %s\n", argv[1]);
		return 1;
	}
	
	Token tok;
	int state;
	while (!feof(input)) {
		state = lex(..., &tok);
		if (strlen(tok.Value) != 0)
			printf("(%s, %s)\n", tok.Type, tok.Value);
		else printf("(%s)\n", tok.Type);
	}
	fclose(input);

	return 0;
}

