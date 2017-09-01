// -----------------------------------------------------------------
// This is the header for the StringConversionPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include <string>
#include "..\stringConverter.h"
#include "..\FGRPhases\fgrStringConversionPhase.h"
using namespace std;

// ----------------------------------------------------------
// This class facilitates converting string literals from 
// frogger syntax to c++ syntax.
//
// Version 5.0
// ----------------------------------------------------------
class StringConversionPhase : SCFPhase
{
private:
	StringConverter converter;
	FGRStringConversionPhase fscp;

protected:
	void processODF(DataCollection * data)
	{
		int dataCount = data->size();
		for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
		{
			DataRecord * currData = data->at(dataIndex);
			if (currData->type->isString())
				currData->defaultValue = converter.convertString(currData->defaultValue);
		}
	}

	void processUDF(UDFRecord * udf)
	{
		udf->root->accept(&fscp);
	}

	void processOF(ObjectStruct * obj)
	{
		if (obj->isUserDefined)
		{
			processODF(obj->data);
			runForLocalUDFs(obj);
			runForLocalOFs(obj);
		}
	}

public:
	void process(ProgramStruct * prog)
	{
		processUDF(prog->PEF);
		processOF(prog);
	}
};