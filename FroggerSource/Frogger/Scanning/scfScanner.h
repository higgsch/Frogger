// -----------------------------------------------------------------
// This is the header file for the SCFScanner class
// -----------------------------------------------------------------
#pragma once

#include "scanner.h"
using namespace std;

// ----------------------------------------------------------
// This class reads through a .struct file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class SCFScanner : public Scanner
{
private:
	void lexical_error(string msg) { lex_error("STRUCT", msg); }

protected:
	char get() { return source.get(); }
	void unget() { source.unget(); }
	char peek() { return source.peek(); }

public:
	Token scan();
};