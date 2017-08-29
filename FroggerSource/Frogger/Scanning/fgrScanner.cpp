//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .fgr file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "fgrScanner.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function opens the input file stream and initializes
// the obfuscator.
// @filename: The .fgr file to open (from project directory).
//
// Version 4.0
// ----------------------------------------------------------
void FGRScanner::openAndInitialize(string filename)
{
	open(filename);
	checkForEmptyFile();
	initializeObfuscator();
}

// ----------------------------------------------------------
// This function closes the input file stream and terminates
// the obfuscator.
//
// Version 4.0
// ----------------------------------------------------------
void FGRScanner::closeAndTerminate()
{
	close();
	terminateObfuscator();
}

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
Token FGRScanner::scan()
{
	char in_char = peek();

	Token foundToken = Token::NOTOK;

	while (in_char != EOF)
	{
		in_char = peek();

		if (readIgnoredChars())
		{
			in_char = peek();
			continue;
		}

		if (isalpha(in_char)) //Identifiers
			return readId();
		else if (isdigit(in_char)) //Double literals
			return readDouble();
		else if (issinglequote(in_char)) //Strings are single quoted
			return readString();
		

		if (foundToken.type == TT_NOTOK)
			foundToken = readBooleanOperator();

		if (foundToken.type == TT_NOTOK)
			foundToken = readArithmeticOperator(); //must be after readBooleanOperator

		if (foundToken.type == TT_NOTOK)
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
// This function throws a lexical error if the input file is
// empty.
//
// Version 3.0
// ----------------------------------------------------------
void FGRScanner::checkForEmptyFile()
{
	char first = source.peek();
	if (first == EOF)
		lexical_error("Empty Input File");
}

// ----------------------------------------------------------
// This function determines if the obfuscator should be used.
//
// Version 3.0
// ----------------------------------------------------------
void FGRScanner::checkForObfuscation()
{
	//Don't care if there are empty comments or not
	readEmptyComments(); 
	
	char newLine = source.get();

	if (newLine == '\n')
	{
		lineNo++;
		obfuscated = true;
	}
	else //first non-empty_comment character is not a newline; thus non-obfuscated
	{
		source.unget();
		obfuscated = false;
	}
}

// ----------------------------------------------------------
// This function sets up the obfuscator.
//
// Version 3.0
// ----------------------------------------------------------
void FGRScanner::initializeObfuscator()
{
	checkForObfuscation();
	if (obfuscated == true)
		obfus = new Obfuscator(&source);
}

// ----------------------------------------------------------
// This function tears down the obfuscator.
//
// Version 3.0
// ----------------------------------------------------------
void FGRScanner::terminateObfuscator()
{
	if (obfus != NULL)
		delete obfus;
}

// ----------------------------------------------------------
// This function reads through a list of empty comments.
// Returns whether or not empty comments were read
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readEmptyComments()
{
	bool hadEmptyComment = false;
	bool isEmptyComment = true;

	while (isEmptyComment)
	{
		isEmptyComment = readThisString("~~");

		if (isEmptyComment)
		{
			hadEmptyComment = true;
		}
	}

	return hadEmptyComment;
}

// ----------------------------------------------------------
// This function attempts to read the next group of ignored  
// chars from the input file.
// Returns whether or not an ignored group was read
// Note: If false is returned, the input location was untouched
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readIgnoredChars()
{
	char in_char = peek();

	//Windows text files contain '\n' as part of the end of line sequence.
	//Other representations may require a '\r' instead to count lines properly.
	if (readThisString("\n"))
	{
		lineNo++;
		return true;
	}
	else if (isspace(in_char))
	{
		string toRead = "";
		toRead += in_char;
		readThisString(toRead);
		return true;
	}
	else if (readThisString("~")) //Comments
	{
		if (!readUntilThisString("~"))
			lexical_error("Unterminated comment");

		return true;
	}

	return false;
}

// ----------------------------------------------------------
// This function gets and returns the next char from the source 
// code dependant upon the obfuscator.
// 
// Version 1.1
// ----------------------------------------------------------
char FGRScanner::get()
{
	if (obfuscated)
		return obfus->get();
	else
		return source.get();
}

// ----------------------------------------------------------
// This function puts the previous char from the source back
// into the source to be retrieved again by get().
// 
// Version 1.1
// ----------------------------------------------------------
void FGRScanner::unget()
{
	if (obfuscated)
		obfus->unget();
	else
		source.unget();
}

// ----------------------------------------------------------
// This function gets and returns the next char from the source 
// code dependant upon the obfuscator. 
// NOTE: Reading location is left untouched
// 
// Version 3.0
// ----------------------------------------------------------
char FGRScanner::peek()
{
	if (obfuscated)
	{
		return obfus->peek();
	}
	else
		return source.peek();
}