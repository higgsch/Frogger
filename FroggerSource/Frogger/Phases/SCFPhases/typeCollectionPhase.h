// -----------------------------------------------------------------
// This is the header for the TypeCollectionPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This class collects all defined object types. 
//
// Version 5.0
// ----------------------------------------------------------
class TypeCollectionPhase : SCFPhase
{
private:
	bool isProcessingProgramObject;
	DataTypeCollection * types;
	string scope;

protected:
	void processUDF(UDFRecord * udf) {}

	void processOF(ObjectStruct * obj) 
	{
		if (obj->isUserDefined)
		{
			if (isProcessingProgramObject)
			{
				isProcessingProgramObject = false;
				runForLocalOFs(obj);
			}
			else
			{
				types->add(scope + obj->name);
		
				string oldScope = scope;
				scope = scope + obj->name + ":";
				runForLocalOFs(obj);
				scope = oldScope;
			}
		}
	}

public:
	void process(ProgramStruct * prog)
	{
		scope = "";
		types->add("double");
		types->add("string");
		types->add("stringList");
		isProcessingProgramObject = true;
		processOF(prog);
	}

	DataTypeCollection * getTypeList()
	{
		return types;
	}

	TypeCollectionPhase() : types(new DataTypeCollection(true)), isProcessingProgramObject(false) {}
};