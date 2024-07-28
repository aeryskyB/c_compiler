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

FILE *input;
// char *text;

void lex(Token *tok);
int isWhitespace(char c);

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
		lex(&tok);
		if (strlen(tok.Value) != 0)
			printf("(%s, %s)\n", tok.Type, tok.Value);
		else if (strlen(tok.Type) != 0) printf("(%s)\n", tok.Type);
	}
	fclose(input);

	return 0;
}

void lex(Token *tok) {
	char c;

	// read 1st character
	// eat potential whitespace(s) on the way
	do {
		c = fgetc(input);
	} while (isWhitespace(c));

	switch (c) {
		case '+':
			strcpy(tok->Type, PLUS_TOK);
			strcpy(tok->Value, "\0");
			// +=, ++ (later)
			return;
		case '-':
			strcpy(tok->Type, MINUS_TOK);
			strcpy(tok->Value, "\0");
			// -=, -- (later)
			return;
		case '*':
			strcpy(tok->Type, MULT_TOK);
			strcpy(tok->Value, "\0");
			// treat pointer type during "data type" declaration
			return;
		case '/':
			strcpy(tok->Type, DIV_TOK);
			strcpy(tok->Value, "\0");
			// don't remember
			return;
		case '%':
			strcpy(tok->Type, MOD_TOK);
			strcpy(tok->Value, "\0");
			// treat placeholder during string type prompts
			return;
		case EOF:
			strcpy(tok->Type, "\0");
			strcpy(tok->Value, "\0");
			return;
	}

	// switch accordingly, and manage control flow
	// return if valid, or invalid token
}

int isWhitespace(char c) {
	// space, tab, line-feed, carriage return, etc.
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
