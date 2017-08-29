//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .struct file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "scfScanner.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
Token SCFScanner::scan()
{
	char in_char = source.peek();

	Token foundToken = Token::NOTOK;

	while (in_char != EOF)
	{
		in_char = source.peek();

		if (isalpha(in_char)) //Identifiers
			return readId();

		foundToken = readPunctuation();
		
		if (foundToken.type != TT_NOTOK)
			return foundToken;
		else
		{
			string msg = "Invalid symbol - '";
			msg += in_char;
			msg += "'";
			lexical_error(msg);
		}
	}

	return Token::SCANEOF;
}