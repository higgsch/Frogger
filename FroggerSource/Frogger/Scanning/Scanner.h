// -----------------------------------------------------------------
// This is the header file for the Scanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "token.h"
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
private:
	Buffer token_buffer; //a buffer to build the current token
	string currFileName;

	bool readIdCharsToBuffer();
	bool readDigitsToBuffer();
	bool readStringToBuffer();
	bool readThisOperator(Token op, string opName);

protected:
	ifstream source; //an input stream for the source file
	int lineNo; //a count variable for the current line number

	bool readThisString(string toRead);
	bool readThisToken(Token tok) { return readThisString(tok.lexeme); }
	bool readUntilThisString(string toRead);

	Token readId();
	Token readDouble();
	Token readString();
	Token readBooleanOperator();
	Token readArithmeticOperator();
	Token readPunctuation();

	virtual char get() =0;
	virtual void unget() =0;
	virtual char peek() =0;
		
	void lex_error(string typeOfLexicalError, string msg);
	bool issinglequote(char c) { return c == '\''; }

public:
	Scanner() : lineNo(1), currFileName("") {}

	virtual Token scan() =0;

	void open(string inFile);
	void close();
	bool good() { return source.good(); }

	int getLineNo() { return lineNo; }
};