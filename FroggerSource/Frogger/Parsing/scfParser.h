// -----------------------------------------------------------------
// This is the header file for the SCFParser class
// -----------------------------------------------------------------
#pragma once

#include "parser.h"
#include "..\Scanning\scfScanner.h"
#include "odfParser.h"
#include "..\DataStructures\Tables\tables.h"
#include "..\DataStructures\Tables\structs.h"
#include "..\DataStructures\dataTyped.h"
#include "..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the functionality to interpret a .struct 
// file
//
// Version 5.0
// ----------------------------------------------------------
class SCFParser : public Parser
{
private:
	string scope;
	DataTypeCollection * types;

	SCFScanner scanner;
	
	UDFRecord * functRecord(string name);
	ObjectStruct * objectRecord(string objectDir, string name);
	DataCollection * dataRecord(string dataDir, string name);
	ArgList * arguments();
	ArgPair * argument();
	DataType * dataType();

	Language * lang;

	void syntax_error(string msg) { syn_error("STRUCT", msg); }

	bool isPEF(UDFRecord * rec, string pefName);
	bool isInFunctions(UDFRecord * rec, UDFCollection * functions);
	bool isInObjects(ObjectStruct * rec, OFCollection * objects);
	bool functionSignatureMatches(UDFRecord * first, UDFRecord * second);

	ObjectStruct * parseObjectLevelSCF(string objectDir, string objectName, string newScope);

	void addEndCommand(UDFRecord* rec);

public:
	SCFParser(Language * lang) : Parser(&scanner), lang(lang), types(new DataTypeCollection()) {}

	ProgramStruct * parseProgramLevelSCF(string projectDir, string projectName);
};