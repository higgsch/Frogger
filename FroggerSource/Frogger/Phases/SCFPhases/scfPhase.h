// -----------------------------------------------------------------
// This is the header for the SCFPhase class.
// -----------------------------------------------------------------
#pragma once

#include "..\..\DataStructures\Tables\structs.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This class provides the base of the visitor inheritance for
// the SCF intermediate representation. 
//
// Version 5.0
// ----------------------------------------------------------
class SCFPhase
{
protected:
	virtual void processUDF(UDFRecord * udf) = 0;
	virtual void processOF(ObjectStruct * obj) = 0;

	// ----------------------------------------------------------
	// This function calls processOF for each OF in the given
	// Object.
	//
	// Version 5.0
	// ----------------------------------------------------------
	void runForLocalOFs(ObjectStruct * obj)
	{
		int objCount = obj->getNumberOfOFs();
		for (int objIndex = 0; objIndex < objCount; objIndex++)
		{
			processOF(obj->getOF(objIndex));
		}
	}

	// ----------------------------------------------------------
	// This function calls processUDF for each UDF in the given
	// Object.
	//
	// Version 5.0
	// ----------------------------------------------------------
	void runForLocalUDFs(ObjectStruct * obj)
	{
		int udfCount = obj->getNumberOfUDFs();
		for (int udfIndex = 0; udfIndex < udfCount; udfIndex++)
		{
			processUDF(obj->getUDF(udfIndex));
		}
	}

public:
	virtual void process(ProgramStruct * prog) = 0;
};