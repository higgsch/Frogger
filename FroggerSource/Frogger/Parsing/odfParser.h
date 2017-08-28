// -----------------------------------------------------------------
// This is the header file for the ODFParser class
// -----------------------------------------------------------------
#pragma once

#include "parser.h"
#include "..\Scanning\odfScanner.h"
#include "..\Scanning\odfToken.h"
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
	ODFToken current_token;
	ODFToken lookahead[1];
	
	DataRecord * dataRecord();
	string defaultValue();
	string id();
	DataType * dataType();

	void match(odf_token_type);
	void syntax_error(string msg) { syn_error("STRUCT", msg); }

	ODFToken next_token();

	bool isInData(DataRecord * rec, DataCollection * data);

public:
	ODFParser(string scope);

	DataCollection * parseODF(string dir, string name);
};