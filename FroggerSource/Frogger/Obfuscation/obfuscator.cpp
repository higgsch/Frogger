//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.2
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
// Version 1.1
// ----------------------------------------------------------
Obfuscator::Obfuscator(ifstream* ifs)
{
	source = ifs;
	buffer = ""; //buffer starts empty
	bufferIndex = 0; //the current location in the buffer is 0
	//no identifiers or keywords have been read yet
	idCounter = 0; 
	keywordCounter = 0;
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
// This function takes a char and returns the char incremented
// by the given integer according to the incrementChar function.
// @c: The char to "increment" through the cycle.
// @by: The number of places in the cycle to increment by.
//
// Version 1.1
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
// Version 2.2
// ----------------------------------------------------------
bool Obfuscator::isKeyword(string s)
{
	return !s.compare("display") || !s.compare("end") || !s.compare("retrieve")
		|| !s.compare("if") || !s.compare("then") || !s.compare("else")
		|| !s.compare("random");
}

// ----------------------------------------------------------
// This function fills the buffer with the remaining chars in
// the string literal.
//
// Version 1.1
// ----------------------------------------------------------
void Obfuscator::fillStringBuffer(void)
{
	int in_char;
	while ((in_char = source->get()) != '\'')
	{
		if (in_char == '&')
		{
			//copy both '&' and the escaped char
			buffer += in_char; 
			in_char = source->get();
		}

		//straight copy; string literals are not obfuscated
		buffer += in_char;
	}

	buffer += in_char; //end of string ('\'')
}

// ----------------------------------------------------------
// This function fills the buffer the next de-obfuscated token.
//
// Version 1.1
// ----------------------------------------------------------
void Obfuscator::fillBuffer(void)
{
	//restart the buffer
	bufferIndex = 0;
	buffer = "";

	int in_char;
	//if the next sequence represents an id or keyword,
	//copy it to the buffer
	while (isIdChar(in_char = source->get()))
		buffer += in_char;

	if (buffer.length() == 0) 
	{ //non-id sequence
		buffer += in_char;

		//test for comments
		if (in_char == '~')
		{
			while((in_char = source->get()) != '~')
				buffer += in_char;

			buffer += in_char; //add the closing '~'
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
		return;
	}
	//id or keyword sequence is in the buffer

	source->unget();

	//determine if the sequence is an obfuscated keyword or id
	string keyword = obfuscateString(buffer, keywordCounter + 1);
	if (isKeyword(keyword))
	{ //valid keyword
		buffer = keyword;
		keywordCounter++;
		return;
	}
	
	//sequence must be an identifier
	//scanner will catch invalid identifiers
	string id = obfuscateString(buffer, idCounter + 1);
	buffer = id;
	idCounter++;
}

// ----------------------------------------------------------
// This function fills the previous buffer to handle ungets
// beyond original buffer boundaries.
//
// Version 1.1
// ----------------------------------------------------------
void Obfuscator::fillPrevBuffer(void)
{
	//prevBuffer is expected to be reverse order of buffer history
	for (int i = 0; i < buffer.length(); i++)
		prevBuffer = buffer[i] + prevBuffer;

	//only allow 4 ungets in a row 
	//this keeps the string well below the maximum allowable length
	if (prevBuffer.length() > 4)
		prevBuffer = prevBuffer.substr(0,4);
}

// ----------------------------------------------------------
// This function returns the next de-obfuscated char from the 
// input file stream.
//
// Version 1.1
// ----------------------------------------------------------
char Obfuscator::get(void)
{
	//check for a buffer that has been read through and needs refilling
	if (bufferIndex == buffer.length())
	{
		fillPrevBuffer();
		fillBuffer();
	}

	//check for ungets beyond buffer boundary
	if (bufferIndex < 0)
	{
		//increment bufferIndex and then negate (-1 -> 0, -2 -> 1)
		return prevBuffer[-++bufferIndex];
	}
	else
		return buffer[bufferIndex++];
}

// ----------------------------------------------------------
// This function puts the previously read char back into the 
// to be read again.
// Note: History is limited to 4 chars beyond id, keyword, or 
// string literal boundaries. Otherwise, only 4 chars.
//
// Version 1.1
// ----------------------------------------------------------
void Obfuscator::unget(void)
{
	bufferIndex--;
}