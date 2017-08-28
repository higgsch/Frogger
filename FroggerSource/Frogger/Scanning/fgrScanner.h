// -----------------------------------------------------------------
// This is the header file for the FGRScanner class
// -----------------------------------------------------------------
#pragma once

#include "Scanner.h"
#include "obfuscator.h"
using namespace std;

//forward declaration
class FGRToken;

// ----------------------------------------------------------
// This class reads through a .fgr file and converts strings
// of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class FGRScanner : public Scanner
{
private:
	Obfuscator* obfus; //a pointer to the obfuscator
	bool obfuscated; //a flag for the obfuscator

	void checkForEmptyFile();
	void checkForObfuscation();

	void initializeObfuscator();
	void terminateObfuscator();

	bool readEmptyComments();
	bool readIgnoredChars();
	FGRToken readId();
	FGRToken readDouble();
	FGRToken readString();
	FGRToken readBooleanOperator();
	FGRToken readArithmeticOperator();
	bool readThisOperator(string op, string opName);
	FGRToken readPunctuation();

	char get();
	void unget();
	char peek();
	
	void lexical_error(string msg) { lex_error("", msg); }

public:
	FGRScanner() : obfuscated(false) {}

	void openAndInitialize(string inFile);
	void closeAndTerminate();

	FGRToken scan();
};