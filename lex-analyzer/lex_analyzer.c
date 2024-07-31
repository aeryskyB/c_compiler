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
int isRelationalLogicalBitwise(char c);
void lex_bin(Token *tok, char c);
void lex_div(Token *tok);
void lex_mult(Token *tok);

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
			return;
		case '-':
			strcpy(tok->Type, MINUS_TOK);
			strcpy(tok->Value, "\0");
			return;
		case '/':
			lex_div(tok);
			return;
		case '*':
			lex_mult(tok);
			return;
		case '%':
			strcpy(tok->Type, MOD_TOK);
			strcpy(tok->Value, "\0");
			// treat placeholder during string type prompts (later)
			return;

		case '^':
			strcpy(tok->Type, XOR_TOK);
			strcpy(tok->Value, "\0");
			return;

		case ';':
			strcpy(tok->Type, SEMICOLON_TOK);
			strcpy(tok->Value, "\0");
			return;

		case ',':
			strcpy(tok->Type, COMMA_TOK);
			strcpy(tok->Value, "\0");
			return;

		case '{':
			strcpy(tok->Type, LBRACE_TOK);
			strcpy(tok->Value, "\0");
			return;
		
		case '}':
			strcpy(tok->Type, RBRACE_TOK);
			strcpy(tok->Value, "\0");
			return;

		case '(':
			strcpy(tok->Type, LPAREN_TOK);
			strcpy(tok->Value, "\0");
			return;

		case ')':
			strcpy(tok->Type, RPAREN_TOK);
			strcpy(tok->Value, "\0");
			return;
		
		case EOF:
			strcpy(tok->Type, "\0");
			strcpy(tok->Value, "\0");
			return;
	}

	if (isRelationalLogicalBitwise(c))
		lex_bin(tok, c);
	
	if (isNamechar(c))
		lex_sp(tok, c);

	if (isNum(c)) 
		lex_num(tok, c);
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

void lex_bin(Token *tok, char c) {
	if (c == '=') {
		c = fgetc(input);
		if (c == '=') {
			strcpy(tok->Type, EQ_TOK);
		}
		else {
			strcpy(tok->Type, ASSIGN_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		strcpy(tok->Value, "\0");
		return;
	}

	if (c == '<') {
		c = fgetc(input);
		if (c == '=')
			strcpy(tok->Type, LE_TOK);
		else if (c == '<')
			strcpy(tok->Type, LSHIFT_TOK);
		else {
			strcpy(tok->Type, LT_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		strcpy(tok->Value, "\0");
		return;
	}

	if (c == '>') {
		c = fgetc(input);
		if (c == '=')
			strcpy(tok->Type, GE_TOK);
		else if (c == '>')
			strcpy(tok->Type, RSHIFT_TOK);
		else {
			strcpy(tok->Type, GT_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		strcpy(tok->Value, "\0");
		return;
	}

	if (c == '!') {
		c = fgetc(input);
		if (c == '=')
			strcpy(tok->Type, NEQ_TOK);
		else {
			strcpy(tok->Type, NOT_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		strcpy(tok->Value, "\0");
		return;
	}

	if (c == '&') {
		c = fgetc(input);
		if (c == '&')
			strcpy(tok->Type, ANDL_TOK);
		else {
			strcpy(tok->Type, ANDB_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		// treat "adress of" later
		// (isNameChar(c))
		strcpy(tok->Value, "\0");
		return;
	}

	if (c == '|') {
		c = fgetc(input);
		if (c == '|')
			strcpy(tok->Type, ORL_TOK);
		else {
			strcpy(tok->Type, ORB_TOK);
			fseek(input, -1, SEEK_CUR);
		}
		strcpy(tok->Value, "\0");
		return;
	}
}

void lex_div(Token *tok) {
	char c_ = fgetc(input);
	if (c_ == '/') { /* a comment */
		do {
			c_ = fgetc(input);
		} while (c_ != '\n' || c_ != EOF);
		strcpy(tok->Type, "\0");
	}
	else if (c_ == '*') {
		strcpy(tok->Type, LCOMMENT_TOK);
	}	
	else {
		strcpy(tok->Type, DIV_TOK);
		fseek(input, -1, SEEK_CUR);
	}
	strcpy(tok->Value, "\0");
}

void lex_mult(Token *tok) {
	char c_ = fgetc(input);
	if (c_ == '/') 
		strcpy(tok->Type, RCOMMENT_TOK);
	else {
		strcpy(tok->Type, DIV_TOK);
		fseek(input, -1, SEEK_CUR);
	}
	strcpy(tok->Value, "\0");
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

int isRelationalLogicalBitwise(char c) {
	return c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|';
}
