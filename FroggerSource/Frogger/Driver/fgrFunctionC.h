// -----------------------------------------------------------------
// This is the header file for the FgrFunctionC class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\DataStructures\Tables\tables.h"
using namespace std;

//forward declaration
class ProgramNode;
struct ProgramStruct;

// ----------------------------------------------------------
// This class compiles a .fgr source file to c++ output.
//
// Version 5.0
// ----------------------------------------------------------
class FgrFunctionC
{
private:
	Language * lang;
	ProgramStruct * progStruct;

	void buildAST(UDFRecord * funct, string inFile);

	void computeJumpToLineNumbers(UDFRecord * funct);
	void convertStrings(UDFRecord * funct);
	void checkDataTypes(UDFRecord * ast);

public:
	FgrFunctionC(Language* lang, ProgramStruct * progStruct) : lang(lang), progStruct(progStruct) {}
	void compileFunctionToAST(string inFile, UDFRecord * rec);
	
	void printAST(UDFRecord * funct, string outFile);
};