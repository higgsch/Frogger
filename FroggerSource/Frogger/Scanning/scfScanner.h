// -----------------------------------------------------------------
// This is the header file for the SCFScanner class
// -----------------------------------------------------------------
#pragma once

#include "Scanner.h"
using namespace std;

//forward declaration
class SCFToken;

// ----------------------------------------------------------
// This class reads through a .struct file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class SCFScanner : public Scanner
{
private:
	SCFToken readId();
	SCFToken readPunctuation();
		
	void lexical_error(string msg) { lex_error("STRUCT", msg); }

protected:
	char get() { return source.get(); }
	void unget() { source.unget(); }
	char peek() { return source.peek(); }

public:
	SCFToken scan();
};