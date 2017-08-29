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
// Version 5.0
// ----------------------------------------------------------
bool Scanner::readThisString(string toRead)
{
	if (toRead[0] != peek())
		return false;

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
// This function attempts to read an identifier from the 
// input file.
// Returns the token for the read identifier or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readId()
{
	token_buffer.reset();

	if (!readIdCharsToBuffer())
		return Token::NOTOK;

	if (token_buffer.contentsEquals(Token::IF.lexeme))
		return Token::IF;
	else if (token_buffer.contentsEquals(Token::THEN.lexeme))
		return Token::THEN;
	else if (token_buffer.contentsEquals(Token::ELSE.lexeme))
		return Token::ELSE;
	else
		return Token(TT_ID, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a double from the 
// input file.
// Returns the token for the read double or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readDouble()
{
	token_buffer.reset();

	if (!readDigitsToBuffer())
		return Token::NOTOK;

	char c = get();
	if (c != '.')
	{
		unget();
		return Token(TT_DOUBLECONST, token_buffer.value());
	}

	token_buffer.append('.');

	if (!readDigitsToBuffer())
		lex_error("", "Missing decimals for double");
	
	return Token(TT_DOUBLECONST, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string from the 
// input file.
// Returns the token for the read string or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readString()
{
	token_buffer.reset();

	if (!readStringToBuffer())
		return Token::NOTOK;

	return Token(TT_STRINGCONST, token_buffer.value());
}

// ----------------------------------------------------------
// This function attempts to read a boolean operator from the 
// input file.
// Returns the token for the read operator or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readBooleanOperator()
{
	if (readThisToken(Token::NOT))
		return Token::NOT;
	else if (readThisToken(Token::DUAL_EQUAL_SIGN))
		return Token::DUAL_EQUAL_SIGN;
	else if (readThisToken(Token::LTE))
		return Token::LTE;
	else if (readThisToken(Token::LT)) //Must be after "<="
		return Token::LT;
	else if (readThisToken(Token::GTE))
		return Token::GTE;
	else if (readThisToken(Token::GT)) //Must be after ">="
		return Token::GT;
	else
		return Token::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read an arithmetic operator from 
// the input file.
// Returns the token for the read operator or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readArithmeticOperator()
{
	if (readThisToken(Token::EQUAL_SIGN)) //Must be after "=="
		return Token::EQUAL_SIGN;
	else if (readThisOperator(Token::DUAL_ADD, "addition or concatenation"))
		return Token::DUAL_ADD;
	else if (readThisOperator(Token::DUAL_SUB, "subtraction"))
		return Token::DUAL_SUB;
	else if (readThisOperator(Token::DUAL_MUL, "multiplication"))
		return Token::DUAL_MUL;
	else if (readThisOperator(Token::DUAL_DIV, "division"))
		return Token::DUAL_DIV;
	else if (readThisOperator(Token::DUAL_MOD, "modulus division"))
		return Token::DUAL_MOD;
	else if (readThisOperator(Token::DUAL_IDIV, "integer division")) //Integer division is \\ (\\\\ in c++ string)
		return Token::DUAL_IDIV;
	else if (readThisOperator(Token::DUAL_ROOT, "rootation"))
		return Token::DUAL_ROOT;
	else if (readThisOperator(Token::DUAL_EXP, "exponentiation"))
		return Token::DUAL_EXP;

	return Token::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read the given operator from the 
// input file.
// Returns whether or not the operator was read, throws a 
// lexical error if the operator is incomplete
//
// Version 5.0
// ----------------------------------------------------------
bool Scanner::readThisOperator(Token op, string opName)
{
	if (peek() != op.firstChar())
		return false;

	if (readThisToken(op))
		return true;
	else
	{
		lex_error("", "Incomplete " + opName + " operator");
		return false;
	}
}

// ----------------------------------------------------------
// This function attempts to read punctuation from the 
// input file.
// Returns the token for the read punctuation or NOTOK
//
// Version 5.0
// ----------------------------------------------------------
Token Scanner::readPunctuation()
{
	if (readThisToken(Token::LPAREN))
		return Token::LPAREN;
	else if (readThisToken(Token::RPAREN))
		return Token::RPAREN;
	else if (readThisToken(Token::EQUAL_SIGN))
		return Token::EQUAL_SIGN;
	else if (readThisToken(Token::COMMA))
		return Token::COMMA;
	else if (readThisToken(Token::TILDE))
		return Token::TILDE;
	else if (readThisToken(Token::DOT))
		return Token::DOT;
	else if (readThisToken(Token::COLON))
		return Token::COLON;
	else if (readThisToken(Token::SEMICOLON))
		return Token::SEMICOLON;
	else if (readThisToken(Token::OCTOTHORPE))
		return Token::OCTOTHORPE;
	//Windows text files contain '\n' as part of the end of line sequence.
	//Other representations may require a '\r' instead to count lines properly.
	else if (readThisString("\n"))
	{
		lineNo++;
		return Token::EOL;
	}
	else 
		return Token::NOTOK;
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

extern bool quietMode;

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @lineNo: The line number that the error occurred on.
// @err_msg: The message to display to the user.
// 
// Version 5.0
// ----------------------------------------------------------
void Scanner::lex_error(string type, string err_msg)
{
	cout << ((type == "") ? "" : type + " ") << "LEXICAL ERROR in file " << currFileName << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}