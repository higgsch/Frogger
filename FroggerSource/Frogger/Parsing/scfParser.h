// -----------------------------------------------------------------
// This is the header file for the SCFParser class
// -----------------------------------------------------------------
#pragma once

#include "..\Scanning\scfScanner.h"
#include "..\Scanning\scfToken.h"
#include "..\DataStructures\dataTyped.h"
#include "..\DataStructures\Tables\tables.h"
#include "..\DataStructures\Tables\structs.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .struct 
// file
//
// Version 5.0
// ----------------------------------------------------------
class SCFParser
{
private:
	string currFilePath;

	SCFScanner scanner;
	SCFToken current_token;
	SCFToken lookahead[1];
	
	UDFRecord * functRecord(string name);
	ObjectStruct * objectRecord(string objectDir, string name);
	ArgList * arguments();
	ArgPair * argument();
	string id();
	DataType * dataType();

	void match(scf_token_type);
	void syntax_error(string);

	SCFToken next_token();

	bool isPEF(UDFRecord * rec, string pefName);
	bool isInFunctions(UDFRecord * rec, UDFCollection * functions);
	bool isInObjects(ObjectStruct * rec, OFCollection * objects);
	bool functionSignatureMatches(UDFRecord * first, UDFRecord * second);

	ObjectStruct * parseObjectLevelSCF(string objectDir, string objectName);

public:
	SCFParser();

	void open(string SCFPath);
	void close() { scanner.close(); }

	ProgramStruct * parseProgramLevelSCF(string projectDir, string projectName);
};