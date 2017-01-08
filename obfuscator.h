// -----------------------------------------------------------------
// This is the header file for the Scanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

// ----------------------------------------------------------
// This class reads through a .fgr file and provides a char 
// stream that has been de-obfuscated per spec.
//
// Version 2.0
// ----------------------------------------------------------
class Obfuscator
{
private:
	ifstream* source; //an input stream for the .fgr code file
	string buffer; //a buffer to build the stream
	string prevBuffer; //a buffer to hold the previous chars (used for successive ungets)
	int bufferIndex; //a buffer pointer
	int idCounter; //a counter for id obfuscation
	int keywordCounter; //a counter for keyword obfuscation

	void fillBuffer(void);
	void fillStringBuffer(void);
	void fillPrevBuffer(void);
	string obfuscateString(string, int);
	char obfuscateChar(char, int);
	char incrementChar(char);
	bool isIdChar(char);
	bool isKeyword(string);

public:
	Obfuscator(ifstream*);
	char get(void);
	void unget(void);
};