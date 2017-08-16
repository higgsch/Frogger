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

extern bool quietMode;

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
			string msg = "Invalid symbol - ";
			msg += in_char;
			lexical_error(msg);
		}
	}

	return ODFToken::SCANEOF;
}

// ----------------------------------------------------------
// This function attempts to read the given string from the 
// input file.
// @toRead: The desire string to read
// Returns whether or not the string was read
// Note: If false is returned, the input location was untouched
//
// Version 5.0
// ----------------------------------------------------------
bool ODFScanner::readThisString(string toRead)
{
	bool matches = true;
	int stringIndex = 0;

	while (matches && stringIndex < toRead.length())
	{
		char c = source.get();
		if (c != toRead[stringIndex]) //works for c == EOF
			matches = false;

		stringIndex++;
	}

	if (matches)
		return true;

	while (stringIndex > 0)
	{
		source.unget();

		stringIndex--;
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

// ----------------------------------------------------------
// This function attempts to read an identifier to the buffer
// from the input file.
// Returns whether or not an identifier was found
// NOTE: The buffer is not reset
//
// Version 5.0
// ----------------------------------------------------------
bool ODFScanner::readIdCharsToBuffer()
{
	char c = source.get();
	bool idRead = isalpha(c);

	while (isalpha(c) || c == '_')
	{
		token_buffer.append(c);
		c = source.get();
	}
	source.unget();

	return idRead;
}

// ----------------------------------------------------------
// This function attempts to read numerical digits to the buffer
// from the input file.
// Returns whether or not a digits were found
// NOTE: The buffer is not reset
//
// Version 5.0
// ----------------------------------------------------------
bool ODFScanner::readDigitsToBuffer()
{
	char c = source.get();
	bool digitsRead = isdigit(c);

	while (isdigit(c))
	{
		token_buffer.append(c);
		c = source.get();
	}
	source.unget();

	return digitsRead;
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string to the buffer
// from the input file.
// Returns whether or not a string was found
// NOTE: The buffer is not reset
//
// Version 5.0
// ----------------------------------------------------------
bool ODFScanner::readStringToBuffer()
{
	char singleQuote = source.get();
	if (!issinglequote(singleQuote))
		return false;

	token_buffer.append(singleQuote);

	char c = source.get();
	while (!issinglequote(c))
	{
		if (c == EOF)
		{
			source.unget();
			lexical_error("Unclosed string literal");
		}
		else if (c != ' ' && isspace(c))
		{
			source.unget();
			lexical_error("Invalid string character");
		}

		token_buffer.append(c);

		if (c == '&') //escape characters
		{
			c = source.get();

			if (c == 't' || c == 'n' || c == '\'' || c == '&')
			{
				token_buffer.append(c);
			}
			else
				lexical_error("Invalid escape sequence");
		}

		c = source.get();
	}

	// c is closing singleQuote

	token_buffer.append(singleQuote);
	return true;
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @lineNo: The line number that the error occurred on.
// @err_msg: The message to display to the user.
// 
// Version 5.0
// ----------------------------------------------------------
void ODFScanner::lexical_error(string err_msg)
{
	cout << "DATA LEXICAL ERROR on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}