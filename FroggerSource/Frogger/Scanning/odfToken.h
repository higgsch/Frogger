// -----------------------------------------------------------------
// This is the header for the ODFToken class. It also includes an enum
// for token categories.
// -----------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <string>
using namespace std;

// set of odf token categories
typedef enum odf_token_types {
	ODFTT_NOTOK = -1, 
	ODFTT_ID, ODFTT_EQUALS, ODFTT_OCTOTHORPE, ODFTT_SEMICOLON,
	ODFTT_STRING, ODFTT_DOUBLECONST,
	ODFTT_EOL, ODFTT_SCANEOF
} odf_token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use and static "constants" for 
// compact use. 
//
// Version 5.0
// ----------------------------------------------------------
class ODFToken
{ //Glorified Struct with public members and static placeholders
public:
	odf_token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	ODFToken() : type(ODFTT_NOTOK), lexeme("<None>") {}
	ODFToken(odf_token_type type, string lexeme) : type(type), lexeme(lexeme) {}

	//static "constants" used for simplified processing of language
	//static tokens
	static ODFToken NOTOK;
	static ODFToken EQUALS;
	static ODFToken OCTOTHORPE;
	static ODFToken SEMICOLON;
	static ODFToken EOL;
	static ODFToken SCANEOF;
};