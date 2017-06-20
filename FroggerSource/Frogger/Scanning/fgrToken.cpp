//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "fgrToken.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
FGRToken::FGRToken()
{
	type = TOKTYPE_NOTOK;
	lexeme = "<None>";
}

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 3.0
// ----------------------------------------------------------
FGRToken::FGRToken(fgr_token_type i_Type, string i_Lexeme)
{
	type = i_Type;
	lexeme = i_Lexeme;
}

//static "constants" used for simplified processing of language
//static tokens
FGRToken FGRToken::NOTOK		= FGRToken(TOKTYPE_NOTOK, "<None>");
FGRToken FGRToken::ASSIGN		= FGRToken(TOKTYPE_ASSIGN, "=");
FGRToken FGRToken::ADD		= FGRToken(TOKTYPE_ADD, "++");
FGRToken FGRToken::SUB		= FGRToken(TOKTYPE_SUB, "--");
FGRToken FGRToken::MUL		= FGRToken(TOKTYPE_MUL, "**");
FGRToken FGRToken::DIV		= FGRToken(TOKTYPE_DIV, "//");
FGRToken FGRToken::MOD		= FGRToken(TOKTYPE_MOD, "%%");
FGRToken FGRToken::IDIV		= FGRToken(TOKTYPE_IDIV, "\\\\");
FGRToken FGRToken::ROOT		= FGRToken(TOKTYPE_ROOT, "##");
FGRToken FGRToken::EXP		= FGRToken(TOKTYPE_EXP, "^^");
FGRToken FGRToken::NOT		= FGRToken(TOKTYPE_NOT, "!");
FGRToken FGRToken::LT			= FGRToken(TOKTYPE_LT, "<");
FGRToken FGRToken::GT			= FGRToken(TOKTYPE_GT, ">");
FGRToken FGRToken::EQ			= FGRToken(TOKTYPE_EQ, "==");
FGRToken FGRToken::LTE		= FGRToken(TOKTYPE_LTE, "<=");
FGRToken FGRToken::GTE		= FGRToken(TOKTYPE_GTE, ">=");
FGRToken FGRToken::IF			= FGRToken(TOKTYPE_IF, "if");
FGRToken FGRToken::THEN		= FGRToken(TOKTYPE_THEN, "then");
FGRToken FGRToken::ELSE		= FGRToken(TOKTYPE_ELSE, "else");
FGRToken FGRToken::COMMA		= FGRToken(TOKTYPE_COMMA, ",");
FGRToken FGRToken::COLON		= FGRToken(TOKTYPE_COLON, ":");
FGRToken FGRToken::SEMICOLON	= FGRToken(TOKTYPE_SEMICOLON, ";");
FGRToken FGRToken::LPAREN		= FGRToken(TOKTYPE_LPAREN, "(");
FGRToken FGRToken::RPAREN		= FGRToken(TOKTYPE_RPAREN, ")");
FGRToken FGRToken::SCANEOF	= FGRToken(TOKTYPE_SCANEOF, "<EOF>");