// -----------------------------------------------------------------
// This is the header file for the ODFParser class
// -----------------------------------------------------------------
#pragma once

#include "..\Scanning\odfScanner.h"
#include "..\Scanning\odfToken.h"
#include "..\DataStructures\dataTyped.h"
#include "..\DataStructures\Tables\structs.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .data
// file
//
// Version 5.0
// ----------------------------------------------------------
class ODFParser
{
private:
	string currFilePath;
	string scope;

	ODFScanner scanner;
	ODFToken current_token;
	ODFToken lookahead[1];
	
	DataRecord * dataRecord();
	string defaultValue();
	string id();
	DataType * dataType();

	void match(odf_token_type);
	void syntax_error(string);

	ODFToken next_token();

	bool isInData(DataRecord * rec, DataCollection * data);

	void open(string SCFPath);
	void close() { scanner.close(); }

public:
	ODFParser(string scope);

	DataCollection * parseODF(string dir, string name);
};