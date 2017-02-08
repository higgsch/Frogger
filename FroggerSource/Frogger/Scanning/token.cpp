//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "token.h"
using namespace std;

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
Token Token::NOTOK		= Token(token_types::NOTOK, "<None>");
Token Token::ASSIGND	= Token(token_types::ASSIGND, "=D=");
Token Token::ADD		= Token(token_types::ADD, "++");
Token Token::SUB		= Token(token_types::SUB, "--");
Token Token::MUL		= Token(token_types::MUL, "**");
Token Token::DIV		= Token(token_types::DIV, "//");
Token Token::MOD		= Token(token_types::MOD, "%%");
Token Token::IDIV		= Token(token_types::IDIV, "\\\\");
Token Token::ROOT		= Token(token_types::ROOT, "##");
Token Token::EXP		= Token(token_types::EXP, "^^");
Token Token::ASSIGNS	= Token(token_types::ASSIGNS, "=S=");
Token Token::CONCATS	= Token(token_types::CONCATS, "+S+");
Token Token::CONCATD	= Token(token_types::CONCATD, "+D+");
Token Token::CONCATA	= Token(token_types::CONCATA, "+A+");
Token Token::NOT		= Token(token_types::NOT, "!");
Token Token::LT			= Token(token_types::LT, "<");
Token Token::GT			= Token(token_types::GT, ">");
Token Token::EQ			= Token(token_types::EQ, "==");
Token Token::LTE		= Token(token_types::LTE, "<=");
Token Token::GTE		= Token(token_types::GTE, ">=");
Token Token::RETRIEVE	= Token(token_types::RETRIEVE, "retrieve");
Token Token::END		= Token(token_types::END, "end");
Token Token::DISPLAY	= Token(token_types::DISPLAY, "display");
Token Token::RANDOM		= Token(token_types::RANDOM, "random");
Token Token::IF			= Token(token_types::IF, "if");
Token Token::THEN		= Token(token_types::THEN, "then");
Token Token::ELSE		= Token(token_types::ELSE, "else");
Token Token::SEMICOLON	= Token(token_types::SEMICOLON, ";");
Token Token::LPAREN		= Token(token_types::LPAREN, "(");
Token Token::RPAREN		= Token(token_types::RPAREN, ")");
Token Token::SCANEOF	= Token(token_types::SCANEOF, "<EOF>");