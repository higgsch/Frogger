//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program reads through a .fgr file and provides a char 
// stream that has been de-obfuscated per spec.
// -----------------------------------------------------------------
#include "obfuscator.h"
using namespace std;

// ----------------------------------------------------------
// This constructor initializes an Obfuscator with the given
// input file stream.
//
// Version 3.0
// ----------------------------------------------------------
Obfuscator::Obfuscator(ifstream* ifs) : outSource(4)
{
	inSource = ifs;

	//no identifiers or keywords have been read yet
	varCounter = 0; 
	keywordCounter = 0;
	routineCounter = 0;
}

// ----------------------------------------------------------
// This function refills the buffer if it has been completely
// read.
//
// Version 3.0
// ----------------------------------------------------------
void Obfuscator::checkEndOfBuffer()
{
	if (outSource.atEndOfBuffer())
	{
		refillOutSource();
	}
}

// ----------------------------------------------------------
// This function returns the next de-obfuscated char from the 
// input file stream.
//
// Version 3.0
// ----------------------------------------------------------
char Obfuscator::get()
{
	checkEndOfBuffer();

	return outSource.get();
}

// ----------------------------------------------------------
// This function puts the previously read char back into the 
// to be read again.
// Note: History is limited to 4 chars beyond id, keyword, or 
// string literal boundaries. Otherwise, only 4 chars.
//
// Version 3.0
// ----------------------------------------------------------
void Obfuscator::unget()
{
	outSource.unget();
}

// ----------------------------------------------------------
// This function returns the next char without moving file
// position.
//
// Version 3.0
// ----------------------------------------------------------
char Obfuscator::peek()
{
	checkEndOfBuffer();
	
	return outSource.peek();
}

// ----------------------------------------------------------
// This function fills the buffer with the next de-obfuscated 
// token.
//
// Version 3.0
// ----------------------------------------------------------
void Obfuscator::refillOutSource()
{
	outSource.archive();
	Buffer buffer;

	char in_char;
	//if the next sequence represents an id or keyword,
	//copy it to the buffer
	while (isIdChar(in_char = inSource->get()))
		buffer.append(in_char);

	if (buffer.isEmpty()) 
	{ //non-id sequence

		//test for comments
		if (in_char == '~')
		{
			buffer.append(in_char);
			while((in_char = inSource->get()) != '~')
				buffer.append(in_char);

			buffer.append(in_char); //add the closing '~'
			outSource.set(buffer.value());
			return;
		}

		if (in_char == '\'')
		{ //string sequence
			fillStringBuffer();
			return;
		}
		//non-string, non-id sequence
		//fill the buffer with only the one char
		//(the scanner builds tokens)
		outSource.set("" + in_char);
		return;
	}
	//id or keyword sequence is in the buffer

	inSource->unget();

	//determine if the sequence is an obfuscated keyword or id
	string keyword = obfuscateString(buffer.value(), keywordCounter + 1);
	if (isKeyword(keyword))
	{ //valid keyword
		outSource.set(keyword);
		keywordCounter++;
		return;
	}

	string routineName = obfuscateString(buffer.value(), routineCounter + 1);
	if (isRoutine(routineName))
	{
		outSource.set(routineName);
		routineCounter++;
		return;
	}
	
	//sequence must be an identifier
	//scanner will catch invalid identifiers
	string id = obfuscateString(buffer.value(), varCounter + 1);
	outSource.set(id);
	varCounter++;
}

// ----------------------------------------------------------
// This function fills the buffer with the remaining chars in
// the string literal.
//
// Version 3.0
// ----------------------------------------------------------
void Obfuscator::fillStringBuffer()
{
	Buffer buffer;
	buffer.set("\'");

	int in_char;
	while ((in_char = inSource->get()) != '\'')
	{
		if (in_char == '&')
		{
			//copy both '&' and the escaped char
			buffer.append(in_char); 
			in_char = inSource->get();
		}

		//straight copy; string literals are not obfuscated
		buffer.append(in_char);
	}

	buffer.append(in_char); //end of string ('\'')
	outSource.set(buffer.value());
}

// ----------------------------------------------------------
// This function takes a string and returns a string that has
// been char-wise incremented by the given integer.
// @s: The string to "increment" through the cycle.
// @by: The number of places in the cycle to increment by.
//
// Version 1.1
// ----------------------------------------------------------
string Obfuscator::obfuscateString(string s, int by)
{
	for (int i = 0; i < s.length(); i++)
	{
		s[i] = obfuscateChar(s[i], by);
	}

	return s;
}

// ---------------------------------------------------------
// This function takes a char and returns the char incremented
// by the given integer according to the incrementChar function.
// @c: The char to "increment" through the cycle.
// @by: The number of places in the cycle to increment by.
//
// Version 3.0
// ----------------------------------------------------------
char Obfuscator::obfuscateChar(char c, int by)
{
	for (int i = 0; i < by; i++)
	{
		c = incrementChar(c);
	}

	return c;
}

// ----------------------------------------------------------
// This function takes a char and returns the next char in 
// the cycle (z-a_Z-A) [z follows A].
// @c: The char to "increment" through the cycle.
//
// Version 1.1
// ----------------------------------------------------------
char Obfuscator::incrementChar(char c)
{
	if (c == 'a') //lowercase boundary
		return '_';
	else if (c == '_') //non-alpha boundary
		return 'Z';
	else if (c == 'A') //uppercase boundary
		return 'z';
	else //non-boundary
		return --c;
}

// ----------------------------------------------------------
// This function returns whether the given char is a valid
// identifier character (alpha or underscore).
// @c: The char in question.
//
// Version 1.1
// ----------------------------------------------------------
bool Obfuscator::isIdChar(char c)
{
	return isalpha(c) || c == '_';
}

// ----------------------------------------------------------
// This function returns whether the given string is a 
// frogger keyword.
// @s: The string in question.
//
// Version 2.5
// ----------------------------------------------------------
bool Obfuscator::isKeyword(string s)
{
	return !s.compare("if") || !s.compare("then") || !s.compare("else");
}

// ----------------------------------------------------------
// This function returns whether the given string is a 
// routine name.
// @s: The string in question.
//
// Version 3.0
// ----------------------------------------------------------
bool Obfuscator::isRoutine(string s)
{
	char nextChar = peek();

	return nextChar == '(';
}