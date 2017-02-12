// -----------------------------------------------------------------
// This is the header file for the Scanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <string>
using namespace std;

// ----------------------------------------------------------
// This class reads through a .fgr file and provides a char 
// stream that has been de-obfuscated per spec.
//
// Version 3.0
// ----------------------------------------------------------
class Obfuscator
{
private:
	ifstream* source; //an input stream for the .fgr code file
	string buffer; //a buffer to build the stream
	string prevBuffer; //a buffer to hold the previous chars (used for successive ungets)
	int bufferIndex; //a buffer pointer
	int varCounter; //a counter for id obfuscation
	int keywordCounter; //a counter for keyword obfuscation
	int routineCounter; // a counter for function and command name obfuscation

	void fillPrevBuffer();
	void fillBuffer();
	void fillStringBuffer();

	string obfuscateString(string, int);
	char obfuscateChar(char, int);
	char incrementChar(char);

	bool isIdChar(char);
	bool isKeyword(string);
	bool isRoutine(string);

public:
	Obfuscator(ifstream*);

	char get();
	void unget();
	char peek();
};