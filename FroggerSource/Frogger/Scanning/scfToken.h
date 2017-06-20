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
	TOKTYPE_NOTOK = -1, 
	TOKTYPE_ID, 
	TOKTYPE_DOT, TOKTYPE_COLON, TOKTYPE_COMMA, TOKTYPE_ARROW, 
	TOKTYPE_EOL, TOKTYPE_SCANEOF
} scf_token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 4.0
// ----------------------------------------------------------
class SCFToken
{ //Glorified Struct with public members and static placeholders
public:
	scf_token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	SCFToken();
	SCFToken(scf_token_type, string);

	//static "constants" used for simplified processing of language
	//static tokens
	static SCFToken NOTOK;
	static SCFToken DOT;
	static SCFToken COLON;
	static SCFToken COMMA;
	static SCFToken ARROW;
	static SCFToken EOL;
	static SCFToken SCANEOF;
};