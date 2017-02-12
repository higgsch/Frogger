// -----------------------------------------------------------------
// This is the header file for the Scanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "..\Obfuscation\obfuscator.h"
using namespace std;

//forward declaration
class Token;

// ----------------------------------------------------------
// This class reads through a .fgr file and converts strings
// of chars to tokens.
//
// Version 3.0
// ----------------------------------------------------------
class Scanner
{
private:
	string token_buffer; //a buffer to build the current token
	ifstream source; //an input stream for the .fgr code file
	Obfuscator* obfus; //a pointer to the obfuscator
	int lineNo; //a count variable for the current line number
	bool obfuscated; //a flag for the obfuscator

	void checkForEmptyFile();
	void checkForObfuscation();

	void initializeObfuscator();
	void terminateObfuscator();

	bool readEmptyComments();
	bool readThisString(string toRead);
	bool readUntilThisString(string toRead);
	bool readIgnoredChars();
	Token readId();
	Token readDouble();
	Token readString();
	Token readBooleanOperator();
	Token readArithmeticOperator();
	bool readThisOperator(string op, string opName);
	Token readPunctuation();

	void resetBuffer() { token_buffer = ""; }
	bool readIdCharsToBuffer();
	bool readDigitsToBuffer();
	bool readStringToBuffer();
	void addToBuffer(char toAdd) { token_buffer += toAdd; }

	char get();
	void unget();
	char peek();
	
	void lexical_error(string msg);

	bool issinglequote(char c) { return c == '\''; }

public:
	Scanner();

	void openAndInitialize(string inFile);
	void closeAndTerminate();

	Token scan();

	int getLineNo() { return lineNo; }
};