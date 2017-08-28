//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .data file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "odfScanner.h"
#include "odfToken.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFScanner::scan()
{
	char in_char = source.peek();

	ODFToken foundToken = ODFToken::NOTOK;

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
		
		if (foundToken.type != ODFTT_NOTOK)
			return foundToken;
		else
		{
			string msg = "Invalid symbol - '";
			msg += in_char;
			msg += "'";
			lexical_error(msg);
		}
	}

	return ODFToken::SCANEOF;
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

// ----------------------------------------------------------
// This function attempts to read an identifier from the 
// input file.
// Returns the token for the read identifier or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFScanner::readId()
{
	token_buffer.reset();

	if (!readIdCharsToBuffer())
		return ODFToken::NOTOK;

	return ODFToken(ODFTT_ID, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a double from the 
// input file.
// Returns the token for the read double or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFScanner::readDouble()
{
	token_buffer.reset();

	if (!readDigitsToBuffer())
		return ODFToken::NOTOK;

	char c = source.get();
	if (c != '.')
	{
		source.unget();
		return ODFToken(ODFTT_DOUBLECONST, token_buffer.value());
	}

	token_buffer.append('.');

	if (!readDigitsToBuffer())
		lexical_error("Missing decimals for double");
	
	return ODFToken(ODFTT_DOUBLECONST, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string from the 
// input file.
// Returns the token for the read string or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFScanner::readString()
{
	token_buffer.reset();

	if (!readStringToBuffer())
		return ODFToken::NOTOK;

	return ODFToken(ODFTT_STRING, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read punctuation from the 
// input file.
// Returns the token for the read punctuation or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFScanner::readPunctuation()
{
	if (readThisString(ODFToken::EQUALS.lexeme))
		return ODFToken::EQUALS;
	else if (readThisString(ODFToken::OCTOTHORPE.lexeme))
		return ODFToken::OCTOTHORPE;
	else if (readThisString(ODFToken::SEMICOLON.lexeme))
		return ODFToken::SEMICOLON;
	//Windows text files contain '\n' as part of the end of line sequence.
	//Other representations may require a '\r' instead to count lines properly.
	else if (readThisString("\n"))
	{
		lineNo++;
		return ODFToken::EOL;
	}
	else
		return ODFToken::NOTOK;
}