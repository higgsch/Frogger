// -----------------------------------------------------------------
// This is the header file for the Compiler class
// -----------------------------------------------------------------
#pragma once

#include <string>
using namespace std;

//forward declaration
class ProgramNode;

// ----------------------------------------------------------
// This class is the driver class for the compiler program.
// It can be used to compile a .fgr source file to c++ output.
//
// Version 3.0
// ----------------------------------------------------------
class Compiler
{
private:
	ProgramNode * root;

	void buildAST(string inFile);

	void setLineNumbers();
	void computeJumpToLineNumbers();
	void convertStrings();
	void checkDataTypes();

	void emitCode(string outFile);

public:
	void run(string inFile, string outFile);
};