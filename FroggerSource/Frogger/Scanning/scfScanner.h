// -----------------------------------------------------------------
// This is the header file for the SCFScanner class
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "..\DataStructures\buffers.h"
using namespace std;

//forward declaration
class SCFToken;

// ----------------------------------------------------------
// This class reads through a .struct file and converts 
// strings of chars to tokens.
//
// Version 5.0
// ----------------------------------------------------------
class SCFScanner
{
private:
	Buffer token_buffer; //a buffer to build the current token
	ifstream source; //an input stream for the .struct code file
	int lineNo; //a count variable for the current line number

	bool readThisString(string toRead);

	SCFToken readId();
	SCFToken readPunctuation();

	bool readIdCharsToBuffer();
		
	void lexical_error(string msg);

public:
	SCFScanner() { lineNo = 1; }

	void open(string inFile) { source.open(inFile); }
	void close() { source.close(); }
	bool good() { return source.good(); }

	SCFToken scan();

	int getLineNo() { return lineNo; }
};