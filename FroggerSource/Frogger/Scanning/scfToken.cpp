//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "scfToken.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 5.0
// ----------------------------------------------------------
SCFToken::SCFToken()
{
	type = SCFTT_NOTOK;
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
SCFToken SCFToken::NOTOK		= SCFToken(SCFTT_NOTOK, "<None>");
SCFToken SCFToken::LPAREN		= SCFToken(SCFTT_LPAREN, "(");
SCFToken SCFToken::RPAREN		= SCFToken(SCFTT_RPAREN, ")");
SCFToken SCFToken::EQUALS		= SCFToken(SCFTT_EQUALS, "=");
SCFToken SCFToken::COMMA		= SCFToken(SCFTT_COMMA, ",");
SCFToken SCFToken::TILDE		= SCFToken(SCFTT_TILDE, "~");
SCFToken SCFToken::DOT			= SCFToken(SCFTT_DOT, ".");
SCFToken SCFToken::EOL			= SCFToken(SCFTT_EOL, "<End of Line>");
SCFToken SCFToken::SCANEOF		= SCFToken(SCFTT_SCANEOF, "<EOF>");