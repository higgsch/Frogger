// -----------------------------------------------------------------
// This is the header for the CodeGenerationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include "..\FGRPhases\fgrCodeGenerationPhase.h"
#include "..\..\Languages\language.h"
#include "..\..\Printing\printer.h"
using namespace std;

// ----------------------------------------------------------
// This class facilitates the FGRCodeGenerationPhase through 
// the SCF.
//
// Version 5.0
// ----------------------------------------------------------
class CodeGenerationPhase : SCFPhase
{
private:
	Printer * p;
	FGRCodeGenerationPhase * fcgp;
	Language * lang;

	void printMetaCode(ProgramStruct * progStruct) { p->printString(lang->getMetaCode(progStruct)); }
	void printPEFCode(ProgramStruct * progStruct);

protected:
	void processUDF(UDFRecord * udf) 
	{
		fcgp->updateCurrentUDFName(udf->name);
		udf->root->accept(fcgp);

		string udfText = udf->root->outputText;
		p->printString(lang->getUDFCode(udf, udfText));
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
	TableGroup * allTables;

	void process(ProgramStruct * prog)
	{
		printMetaCode(prog);
		printPEFCode(prog);
		processOF(prog);
	}

	void open(string filename) { p->open(filename); }
	void close() { p->close(); }

	void outputToExe(string outFilename, string exeFilename) { lang->outputToExe(outFilename, exeFilename); }
	void cleanup(string filename) { lang->cleanup(filename); }

	CodeGenerationPhase(Language * lang) : lang(lang), p(new Printer()), 
		fcgp(new FGRCodeGenerationPhase(lang)) {}
};