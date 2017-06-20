// -----------------------------------------------------------------
// This is the header file for the FGRScanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "obfuscator.h"
#include "..\DataStructures\buffers.h"
using namespace std;

//forward declaration
class FGRToken;

// ----------------------------------------------------------
// This class reads through a .fgr file and converts strings
// of chars to tokens.
//
// Version 4.0
// ----------------------------------------------------------
class FGRScanner
{
private:
	Buffer token_buffer; //a buffer to build the current token
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
	FGRToken readId();
	FGRToken readDouble();
	FGRToken readString();
	FGRToken readBooleanOperator();
	FGRToken readArithmeticOperator();
	bool readThisOperator(string op, string opName);
	FGRToken readPunctuation();

	bool readIdCharsToBuffer();
	bool readDigitsToBuffer();
	bool readStringToBuffer();

	char get();
	void unget();
	char peek();
	
	void lexical_error(string msg);

	bool issinglequote(char c) { return c == '\''; }

public:
	FGRScanner();

	void openAndInitialize(string inFile);
	void closeAndTerminate();

	FGRToken scan();

	int getLineNo() { return lineNo; }
};