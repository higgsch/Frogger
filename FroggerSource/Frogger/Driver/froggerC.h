// -----------------------------------------------------------------
// This is the header file for the FroggerC class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\DataStructures\Tables\tables.h"
#include "..\Languages\CPP\cppLanguage.h"
using namespace std;

//forward declaration
class ProgramNode;
class SCFParser;
class FgrFunctionC;

bool quietMode;

// ----------------------------------------------------------
// This class is the driver class for the compiler program.
// It can be used to compile a Frogger Program Entry Function 
// File or Frogger Project Folder to c++ output.
//
// Version 5.0
// ----------------------------------------------------------
class FroggerC
{
private:
	CPPLanguage * lang;
	ProgramStruct * progStruct;
	SCFParser * p;
	FgrFunctionC * funcComp;

	void verifyPEFExists(string dir);
	void verifyAllContainedUDFsExist(string dir, ObjectStruct * obj);
	void compilePEF(string dir);
	void compileAllContainedUDFs(string dir, ObjectStruct * obj);
	
	void computeRequiredSupportCode(ProgramStruct * prog);

	void emitCode(string dir, string name,string outFile, bool toExe, bool cleanup, bool isProject);

	void verifyFileExists(string filename);
	string getUDFFilename(UDFRecord * udf);
	void struct_error(string err);

public:
	FroggerC::FroggerC(); 

	void compile(string dir, string name, string outFile, bool toExe, bool cleanup, bool isProject);
};