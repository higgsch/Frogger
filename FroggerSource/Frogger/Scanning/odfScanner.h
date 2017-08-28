// -----------------------------------------------------------------
// This is the header file for the ODFScanner class
// -----------------------------------------------------------------
#pragma once

#include "Scanner.h"
using namespace std;

//forward declaration
class ODFToken;

// ----------------------------------------------------------
// This class reads through a .data file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class ODFScanner : public Scanner
{
private:
	bool readIgnoredChars();
	ODFToken readId();
	ODFToken readDouble();
	ODFToken readString();
	ODFToken readPunctuation();
		
	void lexical_error(string msg) { lex_error("DATA", msg); }

protected:
	char get() { return source.get(); }
	void unget() { source.unget(); }
	char peek() { return source.peek(); }

public:
	ODFToken scan();
};