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
	NOTOK = -1, ASSIGN, ADD, SUB, MUL, DIV, STRING,
	ID, DOUBLECONST, RETRIEVE, END, DISPLAY, SEMICOLON, 
	LPAREN, RPAREN, SCANEOF
} token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 1.1
// ----------------------------------------------------------
class Token
{ //Glorified Struct with public members and static placeholders
public:
	token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	Token(){NOTOK,"<None>";}
	Token(token_type, string);

	//static "constants" used for simplified processing of language
	//static tokens
	static Token NOTOK;
	static Token ASSIGN;
	static Token ADD;
	static Token SUB;
	static Token MUL;
	static Token DIV;
	static Token RETRIEVE;
	static Token END;
	static Token DISPLAY;
	static Token SEMICOLON;
	static Token LPAREN;
	static Token RPAREN;
	static Token SCANEOF;
};