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
// Version 5.2
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
				string typeName = scope + obj->name;

				if (obj->isTemplatized())
				{
					typeName += "%" + obj->templatizationList->at(0);

					int tCount = obj->templatizationList->size();
					for (int tIndex = 1; tIndex < tCount; tIndex++)
						typeName += ", " + obj->templatizationList->at(tIndex);

					typeName += "%";
				}

				types->add(typeName);
		
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
		isProcessingProgramObject = true;
		processOF(prog);

		prog->types = types;
	}

	TypeCollectionPhase() : types(new DataTypeCollection(true)), isProcessingProgramObject(false) {}
};