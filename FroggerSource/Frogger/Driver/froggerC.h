// -----------------------------------------------------------------
// This is the header file for the FroggerC class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\DataStructures\Tables\tables.h"
#include "..\DataStructures\OutputLanguage\cppLanguage.h"
using namespace std;

//forward declaration
class ProgramNode;

bool quietMode;

// ----------------------------------------------------------
// This class is the driver class for the compiler program.
// It can be used to compile a Frogger Program Entry Function 
// File or Frogger Project Folder to c++ output.
//
// Version 4.2
// ----------------------------------------------------------
class FroggerC
{
private:
	CPPLanguage * lang;
	ProgramStruct progStruct;

	void computeRequiredSupportCode(ProgramStruct * prog);

	void emitInputFileCode(string outFile);
	void emitInputProjectCode(string outFile);

	void struct_error(string err);

public:
	FroggerC::FroggerC() { 
		progStruct.PEF = new UDFRecord();
		lang = new CPPLanguage();
		lang->initialize();
	}

	void compileInputFile(string inFile, string outFile);
	void compileInputProject(string projectDir, string projectName, string outFile);
};