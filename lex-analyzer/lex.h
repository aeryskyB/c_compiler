#ifndef LEX_H
#define LEX_H

/* Arithmetic */
#define PLUS_TOK '+'
#define MINUS_TOK '-'
#define MULT_TOK '*'
#define DIV_TOK '/'

/* Relational */
#define EQ_TOK "=="
#define LT_TOK '<'
#define LE_TOK "<="
#define GT_TOK '>'
#define GE_TOK ">="
#define NEQ_TOK "!="

/* Logical */
#define ANDL_TOK "&&"
#define ORL_TOK "||"

/* Bitwise */
#define ANDB_TOK "&"
#define ORB_TOK "|"
#define XOR_TOK "^"
#define LSHIFT_TOK "<<"
#define RSHIFT_TOK ">>"

/* Additional */
#define ASSIGN_TOK '='
#define SEMICOLON_TOK ';'

/* Reserved/Keyword */
#define INTK_TOK "INT_KEYWORD"
#define FLOATK_TOK "FLOAT_KEYWORD"
#define CHARK_TOK "CHAR_KEYWORD"
#define IF_TOK "IF_KEYWORD"
#define ELSE_TOK "ELSE_KEYWORD"
#define FOR_TOK "FOR_KEYWORD"
#define WHILE_TOK "WHILE_KEYWORD"

/* Identifier */
#define IDNT_TOK "id"

/* Integer constants */
#define INT_TOK "int"

/* Parens/Braces */
#define LPAREN_TOK "("
#define RPAREN_TOK ")"
#define LBRACE_TOK "{"
#define RBRACE_TOK "}"

#endif
