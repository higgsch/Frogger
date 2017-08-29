// -----------------------------------------------------------------
// This is the header file for the Parser class
// -----------------------------------------------------------------
#pragma once

#include "..\Scanning\scanner.h"
//#include "..\DataStructures\dataTyped.h"
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
	Token current_token; //the currently-selected Token
	Token lookahead[2]; //storage location for two lookahead Tokens

	string currFilePath;

	string id();

	void match(token_type);
	Token next_token();
	Token second_token();

	void syn_error(string, string);

	void open(string path);
	void close() { scanner->close(); }

public:
	Parser(Scanner* scanner);
};