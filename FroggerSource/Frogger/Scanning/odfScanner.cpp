//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .data file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "odfScanner.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
Token ODFScanner::scan()
{
	char in_char = source.peek();

	Token foundToken = Token::NOTOK;

	while (in_char != EOF)
	{
		in_char = source.peek();

		if (readIgnoredChars())
		{
			in_char = source.peek();
			continue;
		}

		if (isalpha(in_char)) //Identifiers
			return readId();
		else if (isdigit(in_char)) //Double literals
			return readDouble();
		else if (issinglequote(in_char)) //Strings are single quoted
			return readString();

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

// ----------------------------------------------------------
// This function attempts to read the next group of ignored  
// chars from the input file.
// Returns whether or not an ignored group was read
// Note: If false is returned, the input location was untouched
//
// Version 5.0
// ----------------------------------------------------------
bool ODFScanner::readIgnoredChars()
{
	char in_char = source.peek();

	if (isspace(in_char) && in_char != '\n')
	{
		string toRead = "";
		toRead += in_char;
		readThisString(toRead);
		return true;
	}

	return false;
}