// -----------------------------------------------------------------
// This is the header file for the ODFParser class
// -----------------------------------------------------------------
#pragma once

#include "parser.h"
#include "..\Scanning\odfScanner.h"
#include "..\DataStructures\Tables\structs.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .data
// file
//
// Version 5.0
// ----------------------------------------------------------
class ODFParser : public Parser
{
private:
	string scope;

	ODFScanner scanner;
	Token current_token;
	Token lookahead[1];
	
	DataRecord * dataRecord();
	string defaultValue();
	string id();
	DataType * dataType();

	void match(token_type);
	void syntax_error(string msg) { syn_error("STRUCT", msg); }

	Token next_token();

	bool isInData(DataRecord * rec, DataCollection * data);

public:
	ODFParser(string scope);

	DataCollection * parseODF(string dir, string name);
};