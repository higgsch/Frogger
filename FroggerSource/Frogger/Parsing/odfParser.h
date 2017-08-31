// -----------------------------------------------------------------
// This is the header file for the ODFParser class
// -----------------------------------------------------------------
#pragma once

#include "parser.h"
#include "..\Scanning\odfScanner.h"
#include "..\DataStructures\Tables\structs.h"
#include "..\DataStructures\dataTyped.h"
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
	
	DataRecord * dataRecord();
	string userDefinedObjectDefaultValue();
	string stringDefaultValue();
	string doubleDefaultValue();
	DataType * dataType();

	void syntax_error(string msg) { syn_error("STRUCT", msg); }

	bool isInData(DataRecord * rec, DataCollection * data);

public:
	ODFParser(string scope) : Parser(&scanner), scope(scope) {}

	DataCollection * parseODF(string dir, string name);
};