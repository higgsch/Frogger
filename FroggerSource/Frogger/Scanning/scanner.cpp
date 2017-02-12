//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program reads through a .fgr file and converts strings of 
// chars to tokens.
// -----------------------------------------------------------------
#include "scanner.h"
#include "token.h"
using namespace std;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 1.1
// ----------------------------------------------------------
Scanner::Scanner()
{
	lineNo = 1; //starts on the first line
	obfuscated = false; //defaults to non-obfuscated
}

// ----------------------------------------------------------
// This function opens the input file stream and initializes
// the obfuscator.
// @filename: The .fgr file to open (from project directory).
//
// Version 3.0
// ----------------------------------------------------------
void Scanner::openAndInitialize(string filename)
{
	source.open(filename);
	checkForEmptyFile();
	initializeObfuscator();
}

// ----------------------------------------------------------
// This function closes the input file stream and terminates
// the obfuscator.
//
// Version 3.0
// ----------------------------------------------------------
void Scanner::closeAndTerminate()
{
	source.close();
	terminateObfuscator();
}

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 3.0
// ----------------------------------------------------------
Token Scanner::scan()
{
	char in_char = peek();

	Token foundToken = Token::NOTOK;

	while (in_char != EOF)
	{
		if (readIgnoredChars())
			continue;

		if (isalpha(in_char)) //Identifiers
			return readId();
		else if (isdigit(in_char)) //Double literals
			return readDouble();
		else if (issinglequote(in_char)) //Strings are single quoted
			return readString();
		

		if (foundToken.type == TOKTYPE_NOTOK)
			foundToken = readBooleanOperator();

		if (foundToken.type == TOKTYPE_NOTOK)
			foundToken = readArithmeticOperator(); //must be after readBooleanOperator

		if (foundToken.type == TOKTYPE_NOTOK)
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

	return Token::SCANEOF;
}

// ----------------------------------------------------------
// This function throws a lexical error if the input file is
// empty.
//
// Version 3.0
// ----------------------------------------------------------
void Scanner::checkForEmptyFile()
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
void Scanner::checkForObfuscation()
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
void Scanner::initializeObfuscator()
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
void Scanner::terminateObfuscator()
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
bool Scanner::readEmptyComments()
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
	int in_char = get();
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

		charReadCount++;
		in_char = get();
	}

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
bool Scanner::readIgnoredChars()
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
		readThisString("" + in_char);
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
// Version 3.0
// ----------------------------------------------------------
Token Scanner::readId()
{
	resetBuffer();

	if (!readIdCharsToBuffer())
		return Token::NOTOK;

	if (token_buffer == "if")
		return Token::IF;
	else if (token_buffer == "then")
		return Token::THEN;
	else if (token_buffer == "else")
		return Token::ELSE;
	else
		return Token(TOKTYPE_ID, token_buffer);
}

// ----------------------------------------------------------
// This function attempts to read a double from the 
// input file.
// Returns the token for the read double or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
Token Scanner::readDouble()
{
	resetBuffer();

	if (!readDigitsToBuffer())
		return Token::NOTOK;

	char c = get();
	if (c != '.')
	{
		unget();
		return Token(TOKTYPE_DOUBLECONST, token_buffer);
	}

	addToBuffer('.');

	if (!readDigitsToBuffer())
		lexical_error("Missing decimals for double");
	
	return Token(TOKTYPE_DOUBLECONST, token_buffer);
}

// ----------------------------------------------------------
// This function attempts to read a FROGGER string from the 
// input file.
// Returns the token for the read string or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
Token Scanner::readString()
{
	resetBuffer();

	if (!readStringToBuffer())
		return Token::NOTOK;

	return Token(TOKTYPE_STRING, token_buffer);
}

// ----------------------------------------------------------
// This function attempts to read a boolean operator from the 
// input file.
// Returns the token for the read operator or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
Token Scanner::readBooleanOperator()
{
	if (readThisString("!"))
		return Token::NOT;
	else if (readThisString("=="))
		return Token::EQ;
	else if (readThisString("<="))
		return Token::LTE;
	else if (readThisString("<")) //Must be after "<="
		return Token::LT;
	else if (readThisString(">="))
		return Token::GTE;
	else if (readThisString(">")) //Must be after ">="
		return Token::GT;
	else
		return Token::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read an arithmetic operator from 
// the input file.
// Returns the token for the read operator or NOTOK
//
// Version 3.0
// ----------------------------------------------------------
Token Scanner::readArithmeticOperator()
{
	int in_char = peek();

	if (readThisString("=")) //Must be after "=="
		return Token::ASSIGN;
	else if (in_char == '+')
		if (readThisOperator("++", "addition or concatenation"))
			return Token::ADD;
	else if (in_char == '-')
		if (readThisOperator("--", "subtraction"))
			return Token::SUB;
	else if (in_char == '*')
		if (readThisOperator("**", "multiplication"))
			return Token::MUL;
	else if (in_char == '/')
		if (readThisOperator("//", "division"))
			return Token::DIV;
	else if (in_char == '%')
		if (readThisOperator("%%", "modulus division"))
			return Token::MOD;
	else if (in_char == '\\')
		if (readThisOperator("\\\\", "integer division")) //Integer division is \\ (\\\\ in c++ string)
			return Token::IDIV;
	else if (in_char == '#')
		if (readThisOperator("##", "rootation"))
			return Token::ROOT;
	else if (in_char == '^')
		if (readThisOperator("^^", "exponentiation"))
			return Token::EXP;

	return Token::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read the given operator from the 
// input file.
// Returns whether or not the operator was read, throws a 
// lexical error if the operator is incomplete
//
// Version 3.0
// ----------------------------------------------------------
bool Scanner::readThisOperator(string op, string opName)
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
Token Scanner::readPunctuation()
{
	if (readThisString("("))
		return Token::LPAREN;
	else if (readThisString(")"))
		return Token::RPAREN;
	else if (readThisString(","))
		return Token::COMMA;
	else if (readThisString(":"))
		return Token::COLON;
	else if (readThisString(";"))
		return Token::SEMICOLON;
	else
		return Token::NOTOK;
}

// ----------------------------------------------------------
// This function attempts to read an identifier to the buffer
// from the input file.
// Returns whether or not an identifier was found
// NOTE: The buffer is not reset
//
// Version 3.0
// ----------------------------------------------------------
bool Scanner::readIdCharsToBuffer()
{
	char c = get();
	bool idRead = isalpha(c);

	while (isalpha(c) || c == '_')
	{
		addToBuffer(c);
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
bool Scanner::readDigitsToBuffer()
{
	char c = get();
	bool digitsRead = isdigit(c);

	while (isdigit(c))
	{
		addToBuffer(c);
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
bool Scanner::readStringToBuffer()
{
	char singleQuote = get();
	if (!issinglequote(singleQuote))
		return false;

	addToBuffer(singleQuote);

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

		addToBuffer(c);

		if (c == '&') //escape characters
		{
			c = get();

			if (c == 't' || c == 'n' || c == '\'' || c == '&')
			{
				addToBuffer(c);
			}
			else
				lexical_error("Invalid escape sequence");
		}

		c = get();
	}

	// c is closing singleQuote

	addToBuffer(singleQuote);
	return true;
}

// ----------------------------------------------------------
// This function gets and returns the next char from the source 
// code dependant upon the obfuscator.
// 
// Version 1.1
// ----------------------------------------------------------
char Scanner::get()
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
void Scanner::unget()
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
char Scanner::peek()
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
// Version 1.0
// ----------------------------------------------------------
void Scanner::lexical_error(string err_msg)
{
	cout << "LEXICAL ERROR on line " << lineNo << ": " << err_msg << endl;
	cout << "Press Enter to Exit" << endl;

	getchar();
	exit(0);
}

//int main(int argc, char * argv[])
//{
//	string filename;
//	if (argc > 1)
//		filename = argv[1];
//	else 
//		filename = "ScannerTest.fgr";
//
//	Scanner s;
//	s.open(filename);
//	Token t = s.scan();
//
//	while (t.type != token_type::SCANEOF)
//	{
//		cout << t.type << ": " << t.lexeme << endl;
//		t = s.scan();
//	}
//
//	cout << "Press Enter to Exit" << endl;
//	getchar();
//}