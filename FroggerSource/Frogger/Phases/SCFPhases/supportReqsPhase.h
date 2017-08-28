// -----------------------------------------------------------------
// This is the header for the SupportReqsPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include "..\FGRPhases\fgrSupportReqsPhase.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class gathers support code requirements.
//
// Version 5.0
// ----------------------------------------------------------
class SupportReqsPhase : SCFPhase
{
private:
	FGRSupportReqsPhase * reqs;

protected:
	void processUDF(UDFRecord * udf) 
	{
		udf->root->accept(reqs);
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
		processUDF(prog->PEF);
		processOF(prog);
	}

	
	SupportReqsPhase(Language * lang) : reqs(new FGRSupportReqsPhase(lang)) {}
	~SupportReqsPhase() { delete reqs; }
};