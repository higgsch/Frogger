// -----------------------------------------------------------------
// This is the header for the InheritancePhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
using namespace std;

// ----------------------------------------------------------
// This class facilitates inheritance checking. 
//
// Version 5.1
// ----------------------------------------------------------
class InheritancePhase : SCFPhase
{
private:
	ProgramStruct * root;

protected:
	void processUDF(UDFRecord * udf) {}
	void processOF(ObjectStruct * obj);

public:
	void process(ProgramStruct * prog);

	InheritancePhase() {}
};