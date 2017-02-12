// -----------------------------------------------------------------
// This is the header for the Token class. It also includes an enum
// for token categories.
// -----------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <string>
using namespace std;

// set of fgr token categories
typedef enum token_types {
	TOKTYPE_NOTOK = -1, 
	TOKTYPE_ASSIGN, 
	TOKTYPE_ADD, TOKTYPE_SUB, TOKTYPE_MUL, TOKTYPE_DIV, 
	TOKTYPE_MOD, TOKTYPE_IDIV, TOKTYPE_ROOT, TOKTYPE_EXP,
	TOKTYPE_NOT, TOKTYPE_LT, TOKTYPE_GT, TOKTYPE_EQ, TOKTYPE_LTE, TOKTYPE_GTE, 
	TOKTYPE_STRING,	TOKTYPE_ID, TOKTYPE_DOUBLECONST, 
	TOKTYPE_IF, TOKTYPE_THEN, TOKTYPE_ELSE, 
	TOKTYPE_COMMA, TOKTYPE_COLON, TOKTYPE_SEMICOLON, TOKTYPE_LPAREN, TOKTYPE_RPAREN, 
	TOKTYPE_SCANEOF
} token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 3.0
// ----------------------------------------------------------
class Token
{ //Glorified Struct with public members and static placeholders
public:
	token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	Token();
	Token(token_type, string);

	//static "constants" used for simplified processing of language
	//static tokens
	static Token NOTOK;
	static Token ASSIGN;
	static Token ADD;
	static Token SUB;
	static Token MUL;
	static Token DIV;
	static Token MOD;
	static Token IDIV;
	static Token ROOT;
	static Token EXP;
	static Token NOT;
	static Token LT;
	static Token GT;
	static Token EQ;
	static Token LTE;
	static Token GTE;
	static Token IF;
	static Token THEN;
	static Token ELSE;
	static Token COMMA;
	static Token COLON;
	static Token SEMICOLON;
	static Token LPAREN;
	static Token RPAREN;
	static Token SCANEOF;
};