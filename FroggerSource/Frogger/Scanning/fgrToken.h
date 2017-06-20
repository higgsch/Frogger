// -----------------------------------------------------------------
// This is the header for the FGRToken class. It also includes an enum
// for token categories.
// -----------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <string>
using namespace std;

// set of fgr token categories
typedef enum fgr_token_types {
	TOKTYPE_NOTOK = -1, 
	TOKTYPE_ASSIGN, 
	TOKTYPE_ADD, TOKTYPE_SUB, TOKTYPE_MUL, TOKTYPE_DIV, 
	TOKTYPE_MOD, TOKTYPE_IDIV, TOKTYPE_ROOT, TOKTYPE_EXP,
	TOKTYPE_NOT, TOKTYPE_LT, TOKTYPE_GT, TOKTYPE_EQ, TOKTYPE_LTE, TOKTYPE_GTE, 
	TOKTYPE_STRING,	TOKTYPE_ID, TOKTYPE_DOUBLECONST, 
	TOKTYPE_IF, TOKTYPE_THEN, TOKTYPE_ELSE, 
	TOKTYPE_COMMA, TOKTYPE_COLON, TOKTYPE_SEMICOLON, TOKTYPE_LPAREN, TOKTYPE_RPAREN, 
	TOKTYPE_SCANEOF
} fgr_token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 3.0
// ----------------------------------------------------------
class FGRToken
{ //Glorified Struct with public members and static placeholders
public:
	fgr_token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	FGRToken();
	FGRToken(fgr_token_type, string);

	//static "constants" used for simplified processing of language
	//static tokens
	static FGRToken NOTOK;
	static FGRToken ASSIGN;
	static FGRToken ADD;
	static FGRToken SUB;
	static FGRToken MUL;
	static FGRToken DIV;
	static FGRToken MOD;
	static FGRToken IDIV;
	static FGRToken ROOT;
	static FGRToken EXP;
	static FGRToken NOT;
	static FGRToken LT;
	static FGRToken GT;
	static FGRToken EQ;
	static FGRToken LTE;
	static FGRToken GTE;
	static FGRToken IF;
	static FGRToken THEN;
	static FGRToken ELSE;
	static FGRToken COMMA;
	static FGRToken COLON;
	static FGRToken SEMICOLON;
	static FGRToken LPAREN;
	static FGRToken RPAREN;
	static FGRToken SCANEOF;
};