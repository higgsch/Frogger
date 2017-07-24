// -----------------------------------------------------------------
// This is the header file for the FgrFunctionC class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\DataStructures\Tables\tables.h"
using namespace std;

//forward declaration
class ProgramNode;

// ----------------------------------------------------------
// This class compiles a .fgr source file to c++ output.
//
// Version 4.2
// ----------------------------------------------------------
class FgrFunctionC
{
private:
	Language * lang;

	void buildAST(FunctionAST * funct, string inFile);

	void computeJumpToLineNumbers(FunctionAST * funct);
	void convertStrings(FunctionAST * funct);
	void checkDataTypes(FunctionAST * ast, FunctionTable * functs, SymbolTable * symbols);

public:
	FgrFunctionC(Language* lang) : lang(lang) {}
	FunctionAST * compileFunctionToAST(string inFile, FunctionTable * functs, UDFRecord * rec);
	
	void printAST(FunctionAST * funct, string outFile);
};