//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program reads through a .fgr file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "fgrScanner.h"
#include "fgrToken.h"
#include <string>
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 4.0
// ----------------------------------------------------------
FGRScanner::FGRScanner()
{
	lineNo = 1; //starts on the first line
	obfuscated = false; //defaults to non-obfuscated
	currFileName = "";
}

// ----------------------------------------------------------
// This function opens the input file stream and initializes
// the obfuscator.
// @filename: The .fgr file to open (from project directory).
//
// Version 4.0
// ----------------------------------------------------------
void FGRScanner::openAndInitialize(string filename)
{
	currFileName = filename;
	source.open(filename);
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
	currFileName = "";
	source.close();
	terminateObfuscator();
}

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRScanner::scan()
{
	char in_char = peek();

	FGRToken foundToken = FGRToken::NOTOK;

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
		

		if (foundToken.type == FGRTT_NOTOK)
			foundToken = readBooleanOperator();

		if (foundToken.type == FGRTT_NOTOK)
			foundToken = readArithmeticOperator(); //must be after readBooleanOperator

		if (foundToken.type == FGRTT_NOTOK)
			foundToken = readPunctuation();

		
		if (foundToken.type != FGRTT_NOTOK)
			return foundToken;
		else
		{
			string msg = "Invalid symbol - ";
			msg += in_char;
			lexical_error(msg);
		}
	}

	return FGRToken::SCANEOF;
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
// This function attempts to read the given string from the 
// input file.
// @toRead: The desire string to read
// Returns whether or not the string was read
// Note: If false is returned, the input location was untouched
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readThisString(string toRead)
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
bool FGRScanner::readUntilThisString(string toRead)
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
// This function attempts to read an identifier from the 
// input file.
// Returns the token for the read identifier or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRScanner::readId()
{
	token_buffer.reset();

	if (!readIdCharsToBuffer())
		return FGRToken::NOTOK;

	if (token_buffer.contentsEquals("if"))
		return FGRToken::IF;
	else if (token_buffer.contentsEquals("then"))
		return FGRToken::THEN;
	else if (token_buffer.contentsEquals("else"))
		return FGRToken::ELSE;
	else
		return FGRToken(FGRTT_ID, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a double from the 
// input file.
// Returns the token for the read double or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRScanner::readDouble()
{
	token_buffer.reset();

	if (!readDigitsToBuffer())
		return FGRToken::NOTOK;

	char c = get();
	if (c != '.')
	{
		unget();
		return FGRToken(FGRTT_DOUBLECONST, token_buffer.value());
	}

	token_buffer.append('.');

	if (!readDigitsToBuffer())
		lexical_error("Missing decimals for double");
	
	return FGRToken(FGRTT_DOUBLECONST, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string from the 
// input file.
// Returns the token for the read string or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRScanner::readString()
{
	token_buffer.reset();

	if (!readStringToBuffer())
		return FGRToken::NOTOK;

	return FGRToken(FGRTT_STRING, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a boolean operator from the 
// input file.
// Returns the token for the read operator or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
FGRToken FGRScanner::readBooleanOperator()
{
	if (readThisString("!"))
		return FGRToken::NOT;
	else if (readThisString("=="))
		return FGRToken::EQ;
	else if (readThisString("<="))
		return FGRToken::LTE;
	else if (readThisString("<")) //Must be after "<="
		return FGRToken::LT;
	else if (readThisString(">="))
		return FGRToken::GTE;
	else if (readThisString(">")) //Must be after ">="
		return FGRToken::GT;
	else
		return FGRToken::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read an arithmetic operator from 
// the input file.
// Returns the token for the read operator or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
FGRToken FGRScanner::readArithmeticOperator()
{
	int in_char = peek();

	if (readThisString("=")) //Must be after "=="
		return FGRToken::ASSIGN;
	else if (in_char == '+')
	{
		if (readThisOperator("++", "addition or concatenation"))
			return FGRToken::ADD;
	}
	else if (in_char == '-')
	{
		if (readThisOperator("--", "subtraction"))
			return FGRToken::SUB;
	}
	else if (in_char == '*')
	{
		if (readThisOperator("**", "multiplication"))
			return FGRToken::MUL;
	}
	else if (in_char == '/')
	{
		if (readThisOperator("//", "division"))
			return FGRToken::DIV;
	}
	else if (in_char == '%')
	{
		if (readThisOperator("%%", "modulus division"))
			return FGRToken::MOD;
	}
	else if (in_char == '\\')
	{
		if (readThisOperator("\\\\", "integer division")) //Integer division is \\ (\\\\ in c++ string)
			return FGRToken::IDIV;
	}
	else if (in_char == '#')
	{
		if (readThisOperator("##", "rootation"))
			return FGRToken::ROOT;
	}
	else if (in_char == '^')
	{
		if (readThisOperator("^^", "exponentiation"))
			return FGRToken::EXP;
	}

	return FGRToken::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read the given operator from the 
// input file.
// Returns whether or not the operator was read, throws a 
// lexical error if the operator is incomplete
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readThisOperator(string op, string opName)
{
	if (peek() != op[0])
		return false;

	if (readThisString(op))
		return true;
	else
	{
		lexical_error("Incomplete " + opName + " operator");
		return false;
	}
}

// ----------------------------------------------------------
// This function attempts to read punctuation from the 
// input file.
// Returns the token for the read punctuation or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
FGRToken FGRScanner::readPunctuation()
{
	if (readThisString("("))
		return FGRToken::LPAREN;
	else if (readThisString(")"))
		return FGRToken::RPAREN;
	else if (readThisString(","))
		return FGRToken::COMMA;
	else if (readThisString(":"))
		return FGRToken::COLON;
	else if (readThisString(";"))
		return FGRToken::SEMICOLON;
	else
		return FGRToken::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read an identifier to the buffer
// from the input file.
// Returns whether or not an identifier was found
// NOTE: The buffer is not reset
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readIdCharsToBuffer()
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
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readDigitsToBuffer()
{
	char c = get();
	bool digitsRead = isdigit(c);

	while (isdigit(c))
	{
		token_buffer.append(c);
		c = get();
	}
	unget();

	return digitsRead;
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string to the buffer
// from the input file.
// Returns whether or not a string was found
// NOTE: The buffer is not reset
//
// Version 3.0
// ----------------------------------------------------------
bool FGRScanner::readStringToBuffer()
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
			lexical_error("Unclosed string literal");
		}
		else if (c != ' ' && isspace(c))
		{
			unget();
			lexical_error("Invalid string character");
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
				lexical_error("Invalid escape sequence");
		}

		c = get();
	}

	// c is closing singleQuote

	token_buffer.append(singleQuote);
	return true;
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

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @lineNo: The line number that the error occurred on.
// @err_msg: The message to display to the user.
// 
// Version 4.4
// ----------------------------------------------------------
void FGRScanner::lexical_error(string err_msg)
{
	cout << "LEXICAL ERROR in file " << currFileName << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}