//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
// -----------------------------------------------------------------
// This program reads through a .struct file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "scfScanner.h"
#include "scfToken.h"
#include <string>
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 4.0
// ----------------------------------------------------------
SCFToken SCFScanner::scan()
{
	char in_char = source.peek();

	SCFToken foundToken = SCFToken::NOTOK;

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

		foundToken = readPunctuation();
		
		if (foundToken.type != TOKTYPE_NOTOK)
			return foundToken;
		else
		{
			string msg = "Invalid symbol - ";
			msg += in_char;
			lexical_error(msg);
		}
	}

	return SCFToken::SCANEOF;
}

// ----------------------------------------------------------
// This function attempts to read the given string from the 
// input file.
// @toRead: The desire string to read
// Returns whether or not the string was read
// Note: If false is returned, the input location was untouched
//
// Version 4.0
// ----------------------------------------------------------
bool SCFScanner::readThisString(string toRead)
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
// This function attempts to read the next group of ignored  
// chars from the input file.
// Returns whether or not an ignored group was read
// Note: If false is returned, the input location was untouched
//
// Version 4.0
// ----------------------------------------------------------
bool SCFScanner::readIgnoredChars()
{
	char in_char = source.peek();

	if (in_char == '\n')
	{
		return false;
	}

	if (isspace(in_char))
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
// Version 4.0
// ----------------------------------------------------------
SCFToken SCFScanner::readId()
{
	token_buffer.reset();

	if (!readIdCharsToBuffer())
		return SCFToken::NOTOK;

	return SCFToken(TOKTYPE_ID, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read punctuation from the 
// input file.
// Returns the token for the read punctuation or NOTOK
//
// Version 4.0
// ----------------------------------------------------------
SCFToken SCFScanner::readPunctuation()
{
	if (readThisString("."))
		return SCFToken::DOT;
	else if (readThisString(":"))
		return SCFToken::COLON;
	else if (readThisString(","))
		return SCFToken::COMMA;
	else if (readThisString("->"))
		return SCFToken::ARROW;
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

// ----------------------------------------------------------
// This function attempts to read an identifier to the buffer
// from the input file.
// Returns whether or not an identifier was found
// NOTE: The buffer is not reset
//
// Version 4.0
// ----------------------------------------------------------
bool SCFScanner::readIdCharsToBuffer()
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
// This function displays an error message to the user and 
// terminates the program.
// @lineNo: The line number that the error occurred on.
// @err_msg: The message to display to the user.
// 
// Version 4.0
// ----------------------------------------------------------
void SCFScanner::lexical_error(string err_msg)
{
	cout << "STRUCT LEXICAL ERROR on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}
}