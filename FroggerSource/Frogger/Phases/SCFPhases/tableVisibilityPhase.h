// -----------------------------------------------------------------
// This is the header for the TableVisibilityPhase class.
// -----------------------------------------------------------------
#pragma once

//#include <string>
//#include <iostream>
//#include "..\..\DataStructures\Tables\structs.h"
#include "scfPhase.h"
using namespace std;

// ----------------------------------------------------------
// This class provides for visible tables management. 
//
// Version 5.0
// ----------------------------------------------------------
class TableVisibilityPhase : SCFPhase
{
private:
	TableGroup * scopedTables;

protected:
	void processUDF(UDFRecord * udf) 
	{
		udf->visibleTables->cmds->merge(scopedTables->cmds);
		udf->visibleTables->functs->merge(scopedTables->functs);
		udf->visibleTables->syms->merge(scopedTables->syms);
	}

	void processOF(ObjectStruct * obj)
	{
		runForLocalOFs(obj);

		scopedTables = obj->scopedTables;
		runForLocalUDFs(obj);
	}

public:
	void process(ProgramStruct * prog)
	{
		processOF(prog);

		scopedTables = prog->scopedTables;
		processUDF(prog->PEF);
	}
};