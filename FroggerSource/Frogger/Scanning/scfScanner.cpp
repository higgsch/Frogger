//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .struct file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "scfScanner.h"
#include "scfToken.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
SCFToken SCFScanner::scan()
{
	char in_char = source.peek();

	SCFToken foundToken = SCFToken::NOTOK;

	while (in_char != EOF)
	{
		in_char = source.peek();

		if (isalpha(in_char)) //Identifiers
			return readId();

		foundToken = readPunctuation();
		
		if (foundToken.type != SCFTT_NOTOK)
			return foundToken;
		else
		{
			string msg = "Invalid symbol - '";
			msg += in_char;
			msg += "'";
			lexical_error(msg);
		}
	}

	return SCFToken::SCANEOF;
}

// ----------------------------------------------------------
// This function attempts to read an identifier from the 
// input file.
// Returns the token for the read identifier or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
SCFToken SCFScanner::readId()
{
	token_buffer.reset();

	if (!readIdCharsToBuffer())
		return SCFToken::NOTOK;

	return SCFToken(SCFTT_ID, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read punctuation from the 
// input file.
// Returns the token for the read punctuation or NOTOK
//
// Version 4.4
// ----------------------------------------------------------
SCFToken SCFScanner::readPunctuation()
{
	if (readThisString(SCFToken::LPAREN.lexeme))
		return SCFToken::LPAREN;
	else if (readThisString(SCFToken::RPAREN.lexeme))
		return SCFToken::RPAREN;
	else if (readThisString(SCFToken::EQUALS.lexeme))
		return SCFToken::EQUALS;
	else if (readThisString(SCFToken::COMMA.lexeme))
		return SCFToken::COMMA;
	else if (readThisString(SCFToken::TILDE.lexeme))
		return SCFToken::TILDE;
	else if (readThisString(SCFToken::DOT.lexeme))
		return SCFToken::DOT;
	//Windows text files contain '\n' as part of the end of line sequence.
	//Other representations may require a '\r' instead to count lines properly.
	else if (readThisString("\n"))
	{
		lineNo++;
		return SCFToken::EOL;
	}
	else
		return SCFToken::NOTOK;
}