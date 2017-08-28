// -----------------------------------------------------------------
// This is the header for the SCFToken class. It also includes an enum
// for token categories.
// -----------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <string>
using namespace std;

// set of scf token categories
typedef enum scf_token_types {
	SCFTT_NOTOK = -1, 
	SCFTT_ID, SCFTT_LPAREN, SCFTT_RPAREN,
	SCFTT_EQUALS, SCFTT_COMMA, SCFTT_TILDE, SCFTT_DOT, 
	SCFTT_EOL, SCFTT_SCANEOF
} scf_token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 5.0
// ----------------------------------------------------------
class SCFToken
{ //Glorified Struct with public members and static placeholders
public:
	scf_token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	SCFToken() : type(SCFTT_NOTOK), lexeme("<None>") {}
	SCFToken(scf_token_type type, string lexeme) : type(type), lexeme(lexeme) {}

	//static "constants" used for simplified processing of language
	//static tokens
	static SCFToken NOTOK;
	static SCFToken LPAREN;
	static SCFToken RPAREN;
	static SCFToken EQUALS;
	static SCFToken COMMA;
	static SCFToken TILDE;
	static SCFToken DOT;
	static SCFToken EOL;
	static SCFToken SCANEOF;
};