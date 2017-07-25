// -----------------------------------------------------------------
// This is the header file for the SCFParser class
// -----------------------------------------------------------------
#pragma once

#include "..\Scanning\scfScanner.h"
#include "..\Scanning\scfToken.h"
#include "..\DataStructures\dataTyped.h"
#include "..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .struct 
// file
//
// Version 4.2
// ----------------------------------------------------------
class SCFParser
{
private:
	SCFScanner scanner;
	SCFToken current_token;
	SCFToken lookahead[1];
	
	UDFRecord * record();
	ArgList * arguments();
	ArgPair * argument();
	string filename();
	DataType dataType();

	void match(scf_token_type);
	void syntax_error(string);

	SCFToken next_token();

	bool isPEF(UDFRecord * rec, string pefName);
	bool isInFiles(UDFRecord * rec, UDFCollection * files);

public:
	SCFParser();

	void open(string SCFPath) { scanner.open(SCFPath); }
	void close() { scanner.close(); }

	UDFCollection * parseSCF(string SCFPath, string projectName);
};