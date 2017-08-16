// -----------------------------------------------------------------
// This is the header file for the ODFScanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "..\DataStructures\buffers.h"
using namespace std;

//forward declaration
class ODFToken;

// ----------------------------------------------------------
// This class reads through a .data file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class ODFScanner
{
private:
	Buffer token_buffer; //a buffer to build the current token
	ifstream source; //an input stream for the .data file
	int lineNo; //a count variable for the current line number

	bool readThisString(string toRead);

	ODFToken readId();
	ODFToken readDouble();
	ODFToken readString();
	ODFToken readPunctuation();

	bool readIdCharsToBuffer();
	bool readDigitsToBuffer();
	bool readStringToBuffer();
		
	void lexical_error(string msg);

	bool issinglequote(char c) { return c == '\''; }

public:
	ODFScanner() { lineNo = 1; }

	void open(string inFile) { source.open(inFile); }
	void close() { source.close(); }
	bool good() { return source.good(); }

	ODFToken scan();

	int getLineNo() { return lineNo; }
};