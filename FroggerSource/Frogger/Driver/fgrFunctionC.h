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
// Version 4.4
// ----------------------------------------------------------
class FgrFunctionC
{
private:
	Language * lang;

	void buildAST(UDFRecord * funct, string inFile);

	void computeJumpToLineNumbers(UDFRecord * funct);
	void convertStrings(UDFRecord * funct);
	void checkDataTypes(UDFRecord * ast, FunctionTable * functs, CommandTable * cmds);

public:
	FgrFunctionC(Language* lang) : lang(lang) {}
	void compileFunctionToAST(string inFile, FunctionTable * functs, CommandTable * cmds, UDFRecord * rec);
	
	void printAST(UDFRecord * funct, string outFile);
};