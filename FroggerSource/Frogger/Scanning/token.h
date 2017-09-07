// -----------------------------------------------------------------
// This is the header for the Token class. It also includes an enum
// for token categories.
// -----------------------------------------------------------------
#pragma once

#include <string>
using namespace std;

// set of scf token categories
typedef enum token_types {
	TT_NOTOK = -1, 
	TT_ID, TT_STRINGCONST, TT_DOUBLECONST, 
	TT_IF, TT_THEN, TT_ELSE,
	TT_LPAREN, TT_RPAREN, TT_EQUAL_SIGN, 
	TT_COLON, TT_DUAL_COLON, TT_SEMICOLON,
	TT_COMMA, TT_OCTOTHORPE, TT_TILDE, TT_DOT, TT_PERCENT,
	TT_DUAL_ADD, TT_DUAL_SUB, TT_DUAL_MUL, TT_DUAL_DIV, 
	TT_DUAL_MOD, TT_DUAL_IDIV, TT_DUAL_ROOT, TT_DUAL_EXP,
	TT_NOT, TT_LT, TT_GT, TT_DUAL_EQUAL_SIGN, TT_LTE, TT_GTE, 
	TT_EOL, TT_SCANEOF
} token_type;

// ----------------------------------------------------------
// This class acts as a struct for tokens but includes 
// constructors for ease of use
//
// Version 5.1
// ----------------------------------------------------------
class Token
{ //Glorified Struct with public members and static placeholders
public:
	token_type type; //The category the token belongs to
	string lexeme; //The textual content of the token

	Token() : type(TT_NOTOK), lexeme("<None>") {}
	Token(token_type type, string lexeme) : type(type), lexeme(lexeme) {}
	char firstChar() { return lexeme[0]; }

	static Token NOTOK;
	static Token IF;
	static Token THEN;
	static Token ELSE;
	static Token LPAREN;
	static Token RPAREN;
	static Token EQUAL_SIGN;
	static Token COLON;
	static Token DUAL_COLON;
	static Token SEMICOLON;
	static Token COMMA;
	static Token OCTOTHORPE;
	static Token TILDE;
	static Token DOT;
	static Token PERCENT;
	static Token DUAL_ADD;
	static Token DUAL_SUB;
	static Token DUAL_MUL;
	static Token DUAL_DIV;
	static Token DUAL_MOD;
	static Token DUAL_IDIV;
	static Token DUAL_ROOT;
	static Token DUAL_EXP;
	static Token NOT;
	static Token LT;
	static Token GT;
	static Token DUAL_EQUAL_SIGN;
	static Token LTE;
	static Token GTE;
	static Token EOL;
	static Token SCANEOF;
};