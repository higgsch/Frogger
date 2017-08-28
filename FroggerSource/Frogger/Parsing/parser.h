// -----------------------------------------------------------------
// This is the header file for the Parser class
// -----------------------------------------------------------------
#pragma once

#include "..\Scanning\Scanner.h"
#include "..\DataStructures\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a file.
//
// Version 5.0
// ----------------------------------------------------------
class Parser
{
protected:
	Scanner * scanner;

	string currFilePath;

	void syn_error(string, string);

	void open(string path);
	void close() { scanner->close(); }

public:
	Parser(Scanner* scanner) : scanner(scanner), currFilePath("") {}
};