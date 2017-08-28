//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "Scanner.h"
#include <string>
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This function opens the input file stream.
// @inFile: The .struct file to open.
//
// Version 5.0
// ----------------------------------------------------------
void Scanner::open(string inFile)
{
	currFileName = inFile;
	source.open(inFile);
}

// ----------------------------------------------------------
// This function closes the input file stream.
//
// Version 5.0
// ----------------------------------------------------------
void Scanner::close()
{
	currFileName = "";
	source.close();
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
bool Scanner::readThisString(string toRead)
{
	bool matches = true;
	int stringIndex = 0;

	while (matches && stringIndex < toRead.length())
	{
		char c = get();
		if (c != toRead[stringIndex]) //works for c == EOF
			matches = false;

		stringIndex++;
	}

	if (matches)
		return true;

	while (stringIndex > 0)
	{
		unget();

		stringIndex--;
	}

	return false;
}

// ----------------------------------------------------------
// This function reads the input file until the given string 
// is read.
// @toRead: The desire string to read until
// Returns whether or not the string was read
// Note: If false is returned, the input location was untouched
//
// Version 3.0
// ----------------------------------------------------------
bool Scanner::readUntilThisString(string toRead)
{
	int toReadIndex = 0;
	int charReadCount = 0;
	int in_char = peek();
	bool stringRead = false;

	while (in_char != EOF && !stringRead)
	{
		if (in_char == '\n')
			lineNo++;

		if (in_char == toRead[toReadIndex])
		{
			toReadIndex++;
			if (toReadIndex == toRead.length())
				stringRead = true;
		}
		else
			toReadIndex = 0;

		in_char = get();
		charReadCount++;
	}
	unget();
	charReadCount--;

	if (stringRead)
		return true;

	for (; charReadCount > 0; charReadCount--)
	{
		unget();
	}

	return false;
}

// ----------------------------------------------------------
// This function attempts to read an identifier to the buffer
// from the input file.
// Returns whether or not an identifier was found
// NOTE: The buffer is not reset
//
// Version 4.0
// ----------------------------------------------------------
bool Scanner::readIdCharsToBuffer()
{
	char c = get();
	bool idRead = isalpha(c);

	while (isalpha(c) || c == '_')
	{
		token_buffer.append(c);
		c = get();
	}
	unget();

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
bool Scanner::readDigitsToBuffer()
{
	char c = get();
	bool digitsRead = isdigit(c);

	while (isdigit(c))
	{
		token_buffer.append(c);
		c = get();
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
bool Scanner::readStringToBuffer()
{
	char singleQuote = get();
	if (!issinglequote(singleQuote))
		return false;

	token_buffer.append(singleQuote);

	char c = get();
	while (!issinglequote(c))
	{
		if (c == EOF)
		{
			unget();
			lex_error("", "Unclosed string literal");
		}
		else if (c != ' ' && isspace(c))
		{
			unget();
			lex_error("", "Invalid string character");
		}

		token_buffer.append(c);

		if (c == '&') //escape characters
		{
			c = get();

			if (c == 't' || c == 'n' || c == '\'' || c == '&')
			{
				token_buffer.append(c);
			}
			else
				lex_error("", "Invalid escape sequence");
		}

		c = get();
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
void Scanner::lex_error(string type_error, string err_msg)
{
	cout << type_error << " LEXICAL ERROR in file " << currFileName << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}