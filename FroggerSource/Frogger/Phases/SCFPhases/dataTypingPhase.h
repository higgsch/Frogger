// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include "..\FGRPhases\fgrDataTypingPhase.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class ensures data typing rules are upheld.
//
// Version 5.0
// ----------------------------------------------------------
class DataTypingPhase : SCFPhase
{
private:
	ProgramStruct * progStruct;

	DataTypeCollection * types;

	Language * lang;

	void populateAllTables(ObjectStruct* obj);

protected:
	void processUDF(UDFRecord * udf) 
	{
		SymbolTable * symbols = new SymbolTable(lang, udf);
		udf->visibleTables->syms->merge(symbols);
		FGRDataTypingPhase dtp(this, lang, types, udf->visibleTables, udf->name);
		udf->root->accept(&dtp);
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
		progStruct = prog;
		types = progStruct->types;

		populateAllTables(prog);

		processUDF(progStruct->PEF);
		processOF(prog);
	}

	TableGroup * getScopedTables(DataType * primary)
	{
		return progStruct->getObject(primary)->scopedTables;
	}

	DataTypingPhase(Language * lang) : lang(lang), allTables(new TableGroup()) {}
};