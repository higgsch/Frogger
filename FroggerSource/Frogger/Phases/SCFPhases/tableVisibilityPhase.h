// -----------------------------------------------------------------
// This is the header for the TableVisibilityPhase class.
// -----------------------------------------------------------------
#pragma once

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
		udf->visibleTables->merge(scopedTables);
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