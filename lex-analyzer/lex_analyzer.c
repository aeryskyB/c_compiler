#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lex.h"

#define TOKEN_TYPE_MAX_LEN 20
#define TOKEN_VAL_MAX_LEN 30
// #define TEXT_LEN 30;

typedef struct {
	char Type[TOKEN_TYPE_MAX_LEN];
	char Value[TOKEN_VAL_MAX_LEN];
} Token;

FILE *input;
// char text[TEXT_LEN];

void lex_head(Token *tok);
int isWhitespace(char c);
int isNamechar(char c);
int isNum(char c);
void lex_sp(Token *tok, char c);
void lex_num(Token *tok, char c);

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
		lex_head(&tok);
		if (strlen(tok.Value) != 0)
			printf("(%s, %s)\n", tok.Type, tok.Value);
		else if (strlen(tok.Type) != 0) printf("(%s)\n", tok.Type);
	}
	fclose(input);

	return 0;
}

void lex_head(Token *tok) {
	char c;
	// read 1st character
	// eat potential whitespace(s) on the way
	do {
		c = fgetc(input);
	} while (isWhitespace(c));

	switch (c) {
		/* arithmetic */
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

		/* relational */
		case '=':
			// call
			return;
		case '<':
			// call
			return;
		case '>':
			// call
			return;
		case '!':
			// call
			return;


		case ';':
			strcpy(tok->Type, SEMICOLON_TOK);
			strcpy(tok->Value, "\0");
			return;

		case EOF:
			strcpy(tok->Type, "\0");
			strcpy(tok->Value, "\0");
			return;
	}
	
	if (isNamechar(c))
		lex_sp(tok, c);

	if (isNum(c)) 
		lex_num(tok, c);

	// switch accordingly, and manage control flow
	// return if valid, or invalid token
}

void lex_sp(Token *tok, char c) {
	int idx = 0;
	do {
		tok->Value[idx++] = c;
		c = fgetc(input);
	} while (isNamechar(c) || isNum(c));
	tok->Value[idx] = '\0';
	fseek(input, -1, SEEK_CUR);

	if (strcmp(tok->Value, "if") == 0) strcpy(tok->Type, IF_TOK);
	else if (strcmp(tok->Value, "else") == 0) strcpy(tok->Type, ELSE_TOK);
	else if (strcmp(tok->Value, "for") == 0) strcpy(tok->Type, FOR_TOK);
	else if (strcmp(tok->Value, "while") == 0) strcpy(tok->Type, WHILE_TOK);
	else if (strcmp(tok->Value, "int") == 0) {
		// implement pointer later!
		strcpy(tok->Type, INTK_TOK);
	}
	else if (strcmp(tok->Value, "char") == 0) {
		// pointer later!
		strcpy(tok->Type, CHARK_TOK);
	}
	else if (strcmp(tok->Value, "float") == 0) {
		// later!
		strcpy(tok->Type, FLOATK_TOK);
	}
	else {
		/* it should be an Id */
		strcpy(tok->Type, IDNT_TOK);
		return;
	}
	tok->Value[0] = '\0';
}

void lex_num(Token *tok, char c) {
	int idx = 0;
	do {
		tok->Value[idx++] = c;
		c = fgetc(input);
	} while (isNum(c));
	tok->Value[idx] = '\0';
	strcpy(tok->Type, INT_TOK);
	fseek(input, -1, SEEK_CUR);
}

int isWhitespace(char c) {
	// space, tab, line-feed, carriage return, etc.
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int isNamechar(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int isNum(char c) {
	return c >= '0' && c <= '9';
}
