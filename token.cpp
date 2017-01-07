//                      Christopher Higgs
//                      CS 6820 - 7:30 am
//                      Final Project
//                      Dr. Rague
//                      Due: 12/10/16
//                      Version: 1.1
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
Token Token::ASSIGN		= Token(token_types::ASSIGN, "==");
Token Token::ADD		= Token(token_types::ADD, "++");
Token Token::SUB		= Token(token_types::SUB, "--");
Token Token::MUL		= Token(token_types::MUL, "**");
Token Token::DIV		= Token(token_types::DIV, "//");
Token Token::RETRIEVE	= Token(token_types::RETRIEVE, "retrieve");
Token Token::END		= Token(token_types::END, "end");
Token Token::DISPLAY	= Token(token_types::DISPLAY, "display");
Token Token::SEMICOLON	= Token(token_types::SEMICOLON, ";");
Token Token::LPAREN		= Token(token_types::LPAREN, "(");
Token Token::RPAREN		= Token(token_types::RPAREN, ")");
Token Token::SCANEOF	= Token(token_types::SCANEOF, "<EOF>");