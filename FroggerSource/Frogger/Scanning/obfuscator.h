// -----------------------------------------------------------------
// This is the header file for the Obfuscator class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <string>
#include "..\DataStructures\buffers.h"
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
	ifstream* inSource; //an input stream for the .fgr code file
	BufferedStream outSource; //an output stream for deobfuscated fgr code

	int varCounter; //a counter for id obfuscation
	int keywordCounter; //a counter for keyword obfuscation
	int routineCounter; // a counter for function and command name obfuscation

	void checkEndOfBuffer();
	void refillOutSource();
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