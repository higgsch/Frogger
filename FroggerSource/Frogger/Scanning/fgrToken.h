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
	FGRTT_NOTOK = -1, 
	FGRTT_ASSIGN, 
	FGRTT_ADD, FGRTT_SUB, FGRTT_MUL, FGRTT_DIV, 
	FGRTT_MOD, FGRTT_IDIV, FGRTT_ROOT, FGRTT_EXP,
	FGRTT_NOT, FGRTT_LT, FGRTT_GT, FGRTT_EQ, FGRTT_LTE, FGRTT_GTE, 
	FGRTT_STRING,	FGRTT_ID, FGRTT_DOUBLECONST, 
	FGRTT_IF, FGRTT_THEN, FGRTT_ELSE, 
	FGRTT_COMMA, FGRTT_COLON, FGRTT_SEMICOLON, FGRTT_LPAREN, FGRTT_RPAREN, 
	FGRTT_SCANEOF
} fgr_token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 5.0
// ----------------------------------------------------------
class FGRToken
{ //Glorified Struct with public members and static placeholders
public:
	fgr_token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	FGRToken() : type(FGRTT_NOTOK), lexeme("<None>") {}
	FGRToken(fgr_token_type type, string lexeme) : type(type), lexeme(lexeme) {}

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