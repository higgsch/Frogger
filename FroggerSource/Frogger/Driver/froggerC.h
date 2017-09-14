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
// Version 5.2
// ----------------------------------------------------------
class FroggerC
{
private:
	CPPLanguage * lang;
	ProgramStruct * progStruct;
	SCFParser * p;
	FgrFunctionC * funcComp;

	void parseSCF(string name, string dir, bool isProject);

	void runScopingPhase();
	void runInheritancePhase();
	void runTableVisibilityPhase();
	void runFileExistencePhase(string dir);
	void runTypeCollectionPhase();
	void runFGRCompilationPhase(string dir);
	void runStringConversionPhase();
	void runDataTypingPhase();
	void runSupportRequirementsPhase();
	void runCodeGenerationPhase(string dir, string name,string outFile, bool toExe, bool cleanup, bool isProject);

	void struct_error(string err);

public:
	FroggerC::FroggerC(); 

	void compile(string dir, string name, string outFile, bool toExe, bool cleanup, bool isProject);
};