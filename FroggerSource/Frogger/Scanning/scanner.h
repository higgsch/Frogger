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

class Token;

// ----------------------------------------------------------
// This class reads through a .fgr file and converts strings
// of chars to tokens.
//
// Version 1.2
// ----------------------------------------------------------
class Scanner
{
private:
	string token_buffer; //a buffer to build the current token
	ifstream source; //an input stream for the .fgr code file
	Obfuscator* obfus; //a pointer to the obfuscator
	int lineNo; //a count variable for the current line number
	bool obfuscated; //a flag for the obfuscator

	void lexical_error(int, string);
	string getString(void);
	char get(void);
	void unget(void);

public:
	Scanner(void);
	~Scanner(void);
	void open(string);
	void checkForObfuscation(void);
	Token scan(void);
	int getLineNo(void);
};