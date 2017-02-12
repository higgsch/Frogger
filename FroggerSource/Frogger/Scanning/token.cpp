//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "token.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
Token::Token()
{
	type = TOKTYPE_NOTOK;
	lexeme = "<None>";
}

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 3.0
// ----------------------------------------------------------
Token::Token(token_type i_Type, string i_Lexeme)
{
	type = i_Type;
	lexeme = i_Lexeme;
}

//static "constants" used for simplified processing of language
//static tokens
Token Token::NOTOK		= Token(TOKTYPE_NOTOK, "<None>");
Token Token::ASSIGN		= Token(TOKTYPE_ASSIGN, "=");
Token Token::ADD		= Token(TOKTYPE_ADD, "++");
Token Token::SUB		= Token(TOKTYPE_SUB, "--");
Token Token::MUL		= Token(TOKTYPE_MUL, "**");
Token Token::DIV		= Token(TOKTYPE_DIV, "//");
Token Token::MOD		= Token(TOKTYPE_MOD, "%%");
Token Token::IDIV		= Token(TOKTYPE_IDIV, "\\\\");
Token Token::ROOT		= Token(TOKTYPE_ROOT, "##");
Token Token::EXP		= Token(TOKTYPE_EXP, "^^");
Token Token::NOT		= Token(TOKTYPE_NOT, "!");
Token Token::LT			= Token(TOKTYPE_LT, "<");
Token Token::GT			= Token(TOKTYPE_GT, ">");
Token Token::EQ			= Token(TOKTYPE_EQ, "==");
Token Token::LTE		= Token(TOKTYPE_LTE, "<=");
Token Token::GTE		= Token(TOKTYPE_GTE, ">=");
Token Token::IF			= Token(TOKTYPE_IF, "if");
Token Token::THEN		= Token(TOKTYPE_THEN, "then");
Token Token::ELSE		= Token(TOKTYPE_ELSE, "else");
Token Token::COMMA		= Token(TOKTYPE_COMMA, ",");
Token Token::COLON		= Token(TOKTYPE_COLON, ":");
Token Token::SEMICOLON	= Token(TOKTYPE_SEMICOLON, ";");
Token Token::LPAREN		= Token(TOKTYPE_LPAREN, "(");
Token Token::RPAREN		= Token(TOKTYPE_RPAREN, ")");
Token Token::SCANEOF	= Token(TOKTYPE_SCANEOF, "<EOF>");