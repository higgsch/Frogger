//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.4
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "scfToken.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 4.0
// ----------------------------------------------------------
SCFToken::SCFToken()
{
	type = TOKTYPE_NOTOK;
	lexeme = "<None>";
}

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 4.0
// ----------------------------------------------------------
SCFToken::SCFToken(scf_token_type i_Type, string i_Lexeme)
{
	type = i_Type;
	lexeme = i_Lexeme;
}

//static "constants" used for simplified processing of language
//static tokens
SCFToken SCFToken::NOTOK		= SCFToken(TOKTYPE_NOTOK, "<None>");
SCFToken SCFToken::LPAREN		= SCFToken(TOKTYPE_LPAREN, "(");
SCFToken SCFToken::RPAREN		= SCFToken(TOKTYPE_RPAREN, ")");
SCFToken SCFToken::EQUALS		= SCFToken(TOKTYPE_EQUALS, "=");
SCFToken SCFToken::COMMA		= SCFToken(TOKTYPE_COMMA, ",");
SCFToken SCFToken::TILDE		= SCFToken(TOKTYPE_TILDE, "~");
SCFToken SCFToken::DOT			= SCFToken(TOKTYPE_DOT, ".");
SCFToken SCFToken::EOL			= SCFToken(TOKTYPE_EOL, "<End of Line>");
SCFToken SCFToken::SCANEOF	= SCFToken(TOKTYPE_SCANEOF, "<EOF>");