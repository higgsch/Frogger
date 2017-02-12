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
// Version 1.0
// ----------------------------------------------------------
Token::Token(token_type aType, string aLexeme)
{
	type = aType;
	lexeme = aLexeme;
}

//static "constants" used for simplified processing of language
//static tokens
Token Token::NOTOK		= Token(token_types::TOKTYPE_NOTOK, "<None>");
Token Token::ASSIGN		= Token(token_types::TOKTYPE_ASSIGN, "=");
Token Token::ADD		= Token(token_types::TOKTYPE_ADD, "++");
Token Token::SUB		= Token(token_types::TOKTYPE_SUB, "--");
Token Token::MUL		= Token(token_types::TOKTYPE_MUL, "**");
Token Token::DIV		= Token(token_types::TOKTYPE_DIV, "//");
Token Token::MOD		= Token(token_types::TOKTYPE_MOD, "%%");
Token Token::IDIV		= Token(token_types::TOKTYPE_IDIV, "\\\\");
Token Token::ROOT		= Token(token_types::TOKTYPE_ROOT, "##");
Token Token::EXP		= Token(token_types::TOKTYPE_EXP, "^^");
Token Token::NOT		= Token(token_types::TOKTYPE_NOT, "!");
Token Token::LT			= Token(token_types::TOKTYPE_LT, "<");
Token Token::GT			= Token(token_types::TOKTYPE_GT, ">");
Token Token::EQ			= Token(token_types::TOKTYPE_EQ, "==");
Token Token::LTE		= Token(token_types::TOKTYPE_LTE, "<=");
Token Token::GTE		= Token(token_types::TOKTYPE_GTE, ">=");
Token Token::IF			= Token(token_types::TOKTYPE_IF, "if");
Token Token::THEN		= Token(token_types::TOKTYPE_THEN, "then");
Token Token::ELSE		= Token(token_types::TOKTYPE_ELSE, "else");
Token Token::COMMA		= Token(token_types::TOKTYPE_COMMA, ",");
Token Token::COLON		= Token(token_types::TOKTYPE_COLON, ":");
Token Token::SEMICOLON	= Token(token_types::TOKTYPE_SEMICOLON, ";");
Token Token::LPAREN		= Token(token_types::TOKTYPE_LPAREN, "(");
Token Token::RPAREN		= Token(token_types::TOKTYPE_RPAREN, ")");
Token Token::SCANEOF	= Token(token_types::TOKTYPE_SCANEOF, "<EOF>");