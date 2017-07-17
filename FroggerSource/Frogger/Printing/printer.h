// -----------------------------------------------------------------
// This is the header for the Printer class.
// -----------------------------------------------------------------
#pragma once

#include <fstream>
#include <string>
using namespace std;

// ----------------------------------------------------------
// This class enables printing.
//
// Version 4.2
// ----------------------------------------------------------
class Printer
{
private:
	ofstream* out; // the output stream to print to

public:
	Printer() : out(new ofstream()) {}

	void open(string filename) { out->open(filename); }
	void close() { out->close(); }

	void printString(string s) { *out << s; }

	void printEmptyLine() { printString("\n"); }
};