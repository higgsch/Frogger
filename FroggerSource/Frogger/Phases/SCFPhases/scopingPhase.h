// -----------------------------------------------------------------
// This is the header for the ScopingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This class refines all DataType::scopes 
//
// Version 5.2
// ----------------------------------------------------------
class ScopingPhase : SCFPhase
{
private:
	void processDT(ObjectStruct * containingObj, DataType * dt)
	{
		if (containingObj != NULL && containingObj->isTemplatized())
		{
			if (containingObj->templatizationList->contains(dt->typeName))
			{
				dt->scope = "";
				return;
			}

			if (containingObj->name == dt->typeName && dt->templatizerList->size() == 0)
			{
				int tCount = containingObj->templatizationList->size();
				for (int tIndex = 0; tIndex < tCount; tIndex++)
					dt->templatizerList->add(containingObj->templatizationList->at(tIndex));
			}
		}
	}

	void processODF(DataCollection * odf)
	{
		int dataCount = odf->size();
		for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
		{
			processDT(odf->containingOF, odf->at(dataIndex)->type);
		}
	}

protected:

	void processUDF(UDFRecord * udf) 
	{
		processDT(udf->containingOF, udf->returnType);
		processDT(udf->containingOF, udf->primary);

		int argCount = udf->getNumArgs();
		for (int argIndex = 0; argIndex < argCount; argIndex++)
		{
			processDT(udf->containingOF, udf->getArg(argIndex)->type);
		}
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

	ScopingPhase(){}
};