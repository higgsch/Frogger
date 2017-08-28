// -----------------------------------------------------------------
// This is the header file for the Scanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "..\DataStructures\buffers.h"
using namespace std;

// ----------------------------------------------------------
// This class reads through a file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class Scanner
{
protected:
	Buffer token_buffer; //a buffer to build the current token
	ifstream source; //an input stream for the source file
	string currFileName;
	int lineNo; //a count variable for the current line number

	bool readThisString(string toRead);
	bool readUntilThisString(string toRead);

	bool readIdCharsToBuffer();
	bool readDigitsToBuffer();
	bool readStringToBuffer();

	virtual char get() =0;
	virtual void unget() =0;
	virtual char peek() =0;
		
	void lex_error(string typeOfLexicalError, string msg);

	bool issinglequote(char c) { return c == '\''; }

public:
	Scanner() : lineNo(1), currFileName("") {}

	void open(string inFile);
	void close();
	bool good() { return source.good(); }

	int getLineNo() { return lineNo; }
};