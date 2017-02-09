//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
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
// This deconstructor makes sure the file stream is closed.
//
// Version 1.0
// ----------------------------------------------------------
Scanner::~Scanner(void)
{
	if (source.is_open())
		source.close();
}

// ----------------------------------------------------------
// This function opens the file stream.
// @filename: The .fgr file to open (from project directory).
//
// Version 1.0
// ----------------------------------------------------------
void Scanner::open(string filename)
{
	source.open(filename);
}

// ----------------------------------------------------------
// This function determines if the obfuscator should be used.
//
// Version 1.1
// ----------------------------------------------------------
void Scanner::checkForObfuscation(void)
{
	char in_char;
	while ((in_char = source.get()) == '~')
	{
		char second = source.get();
		if (second != '~') //has to be empty comment
		{ //not obfuscated
			source.unget(); //first char in comment
			source.unget(); //'~'
			return;
		}
		//empty comment found; continue to next
		continue;
	}
	//only empty comments have been found
	if (in_char == '\n')
	{
		lineNo++;
		obfuscated = true;
		obfus = new Obfuscator(&source);
	}
	else //first non-tilde is not a newline; non-obfuscated
		source.unget();
}

// ----------------------------------------------------------
// This function scans for and returns the next token.
//
// Version 2.3
// ----------------------------------------------------------
Token Scanner::scan(void)
{
	char in_char;

	while ((in_char = get()) != EOF)
	{
		//Windows text files contain '\n' as part of the end of line sequence.
		//Other representations may require a '\r' instead to count lines properly.
		if (in_char == '\n')
		{
			lineNo++;
			continue;
		}
		else if (isspace(in_char))
			continue;
		else if (in_char == '(')
			return Token::LPAREN;
		else if (in_char == ')')
			return Token::RPAREN;
		else if (in_char == ';')
			return Token::SEMICOLON;
		else if (in_char == '=')
		{
			int c = get();
			if (c == '=')
				return Token::EQ;
			else 
			{
				unget();
				return Token::ASSIGN;
			}
		}
		else if (in_char == '!')
			return Token::NOT;
		else if (in_char == '<')
		{
			int c = get();
			if (c == '=')
				return Token::LTE;
			
			//Only < found
			unget();
			return Token::LT;
		}
		else if (in_char == '>')
		{
			int c = get();
			if (c == '=')
				return Token::GTE;

			//Only > found
			unget();
			return Token::GT;
		}
		else if (in_char == '+')
		{
			int c = get();
			if (c == '+')
				return Token::ADD;
			else 
			{
				unget();
				lexical_error(lineNo, "Incomplete addition or concatenation operator");
			}
		}
		else if (in_char == '-')
		{
			int c = get();
			if (c == '-')
				return Token::SUB;
			else //Subtraction is --
			{
				unget();
				lexical_error(lineNo, "Incomplete subtraction operator");
			}
		}
		else if (in_char == '*')
		{
			int c = get();
			if (c == '*')
				return Token::MUL;
			else //Multiplication is **
			{
				unget();
				lexical_error(lineNo, "Incomplete multiplication operator");
			}
		}
		else if (in_char == '/')
		{
			int c = get();
			if (c == '/')
				return Token::DIV;
			else //Division is //
			{
				unget();
				lexical_error(lineNo, "Incomplete division operator");
			}
		}
		
		else if (in_char == '%')
		{
			int c = get();
			if (c == '%')
				return Token::MOD;
			else //Modulus division is %%
			{
				unget();
				lexical_error(lineNo, "Incomplete modulus division operator");
			}
		}
		else if (in_char == '\\')
		{
			int c = get();
			if (c == '\\')
				return Token::IDIV;
			else //Integer division is \\ (\\\\ in c++ string)
			{
				unget();
				lexical_error(lineNo, "Incomplete integer division operator");
			}
		}
		else if (in_char == '#')
		{
			int c = get();
			if (c == '#')
				return Token::ROOT;
			else //Rootation is ##
			{
				unget();
				lexical_error(lineNo, "Incomplete rootation operator");
			}
		}
		else if (in_char == '^')
		{
			int c = get();
			if (c == '^')
				return Token::EXP;
			else //Exponentiation is ^^
			{
				unget();
				lexical_error(lineNo, "Incomplete exponentiation operator");
			}
		}
		else if (in_char == '\'') //Strings are single quoted
		{
			return Token(STRING, getString());
		}
		else if (in_char == '~') //Comments
		{
			int c;
			while ((c = get()) != '~')
			{
				if (c == '\n')
					lineNo++;
				else if (c == EOF)
					lexical_error(lineNo, "Unterminated comment");
			}
			continue;
		}
		else if (isalpha(in_char)) //Identifiers
		{
			token_buffer = in_char;
			int c;
			while (isalpha(c = get()) || c == '_')
			{
				token_buffer += c;
			}

			unget();

			if (token_buffer == "retrieve")
				return Token::RETRIEVE;
			else if (token_buffer == "display")
				return Token::DISPLAY;
			else if (token_buffer == "random")
				return Token::RANDOM;
			else if (token_buffer == "end")
				return Token::END;
			else if (token_buffer == "if")
				return Token::IF;
			else if (token_buffer == "then")
				return Token::THEN;
			else if (token_buffer == "else")
				return Token::ELSE;
			else
				return Token(ID, token_buffer);
		}
		else if (isdigit(in_char)) //Double literals
		{
			token_buffer = in_char;
			int c;
			while (isdigit(c = get()))
			{
				token_buffer += c;
			}

			if (c == '.')
			{
				token_buffer += c;
				if (!isdigit(c = get()))
				{
					unget();
					lexical_error(lineNo, "Missing decimals for double");
				}
				else
					token_buffer += c;

				while (isdigit(c = get()))
				{
					token_buffer += c;
				}
				unget();
				return Token(DOUBLECONST, token_buffer);
			}
			else
			{
				unget();
				return Token(DOUBLECONST, token_buffer);
			}
		}
		else
		{
			string msg = "Invalid symbol - ";
			msg += in_char;
			lexical_error(lineNo, msg);
		}
	}

	return Token::SCANEOF;
}

// ----------------------------------------------------------
// This function scans and returns the remainder of a string 
// literal
//
// Version 1.1
// ----------------------------------------------------------
string Scanner::getString(void)
{
	token_buffer = "\'";
	int c;
	while ((c = get()) != '\'')
	{
		if (c == EOF)
		{
			unget();
			lexical_error(lineNo, "Unclosed string literal");
		}
		else if (c != ' ' && isspace(c))
		{
			unget();
			lexical_error(lineNo, "Invalid string character");
		}

		if (c != '&') //non-escape characters
		{
			token_buffer += c;
		}
		else //escape characters
		{
			c = get();
			token_buffer += '&';

			if (c == 't' || c == 'n' || c == '\'' || c == '&')
			{
				token_buffer += c;
			}
			else
				lexical_error(lineNo, "Invalid escape sequence");
		}
	}

	token_buffer += '\'';
	return token_buffer;
}

// ----------------------------------------------------------
// This function gets and returns the next char from the source 
// code dependant upon the obfuscator.
// 
// Version 1.1
// ----------------------------------------------------------
char Scanner::get(void)
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
void Scanner::unget(void)
{
	if (obfuscated)
		obfus->unget();
	else
		source.unget();
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @lineNo: The line number that the error occurred on.
// @err_msg: The message to display to the user.
// 
// Version 1.0
// ----------------------------------------------------------
void Scanner::lexical_error(int lineNo, string err_msg)
{
	cout << "LEXICAL ERROR on line " << lineNo << ": " << err_msg << endl;
	cout << "Press Enter to Exit" << endl;

	getchar();
	exit(0);
}

// ----------------------------------------------------------
// This function returns the current line number.
//
// Version 1.0
// ----------------------------------------------------------
int Scanner::getLineNo(void)
{
	return lineNo;
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