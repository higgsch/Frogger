// -----------------------------------------------------------------
// This is the header file for the SCFParser class
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "..\Scanning\scfScanner.h"
#include "..\Scanning\scfToken.h"
#include "..\DataStructures\dataTyped.h"
#include "..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .struct 
// file
//
// Version 4.0
// ----------------------------------------------------------
class SCFParser
{
private:
	SCFScanner scanner;
	SCFToken current_token;
	SCFToken lookahead[1];
	
	UDFRecord * record();
	vector<argPair *> * arguments();
	argPair * argument();
	string filename();
	DataType dataType();

	void match(scf_token_type);
	void syntax_error(string);

	SCFToken next_token();

	bool isPEF(UDFRecord * rec, string pefName);

public:
	SCFParser();

	void open(string SCFPath) { scanner.open(SCFPath); }
	void close() { scanner.close(); }

	vector<UDFRecord *> * parseSCF(string SCFPath, string projectName);
};