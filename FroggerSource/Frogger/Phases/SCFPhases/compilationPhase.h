// -----------------------------------------------------------------
// This is the header for the CompilationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include "..\..\Languages\language.h"
#include "..\..\Driver\fgrFunctionC.h"
using namespace std;

// ----------------------------------------------------------
// This class facilitates the FGR compilation front end and 
// middle end. 
//
// Version 5.0
// ----------------------------------------------------------
class CompilationPhase : SCFPhase
{
private:
	Language * lang;
	FgrFunctionC * funcComp;

protected:
	void processUDF(UDFRecord * udf) 
	{
		funcComp->compileFunction(udf);
	}

	void processOF(ObjectStruct * obj) 
	{
		if (obj->isUserDefined)
		{
			runForLocalUDFs(obj);
			runForLocalOFs(obj);
		}
	}

public:
	void process(ProgramStruct * prog)
	{
		funcComp = new FgrFunctionC(lang, prog);
		funcComp->compileFunction(prog->PEF);
		processOF(prog);
		delete funcComp;
	}

	CompilationPhase(Language * lang) : lang(lang) {}
};