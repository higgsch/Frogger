// -----------------------------------------------------------------
// This is the header file for the ODFScanner class
// -----------------------------------------------------------------
#pragma once

#include "scanner.h"
using namespace std;

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
		
	void lexical_error(string msg) { lex_error("DATA", msg); }

protected:
	char get() { return source.get(); }
	void unget() { source.unget(); }
	char peek() { return source.peek(); }

public:
	Token scan();
};