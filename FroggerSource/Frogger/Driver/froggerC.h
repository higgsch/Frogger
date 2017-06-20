// -----------------------------------------------------------------
// This is the header file for the FroggerC class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "..\DataStructures\Tables\tables.h"
using namespace std;

//forward declaration
class ProgramNode;

bool quietMode;

// ----------------------------------------------------------
// This class is the driver class for the compiler program.
// It can be used to compile a Frogger Program Entry Function 
// File or Frogger Project Folder to c++ output.
//
// Version 4.0
// ----------------------------------------------------------
class FroggerC
{
private:
	ProgramAST progAST;
	ProgramStruct progStruct;

	void emitInputFileCode(string outFile);
	void emitInputProjectCode(string outFile);

	void struct_error(string err);

public:
	FroggerC::FroggerC() { 
		progStruct.PEF = new UDFRecord();
		progStruct.UDFs = new vector<UDFRecord *>();
		progAST.UDFs = new vector<FunctionAST *>(); }

	void compileInputFile(string inFile, string outFile);
	void compileInputProject(string projectDir, string projectName, string outFile);
};