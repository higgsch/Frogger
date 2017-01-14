// -----------------------------------------------------------------
// This is the header for the ImportsSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phases.h"
#include "..\..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating import
// statements as a subphase of the CodeGenerationPhase.
//
// Version 2.2
// ----------------------------------------------------------
class IncludesSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	bool isStringImported; // flag for <string>
	bool isIOStreamImported; // flag for <iostream>
	bool isMathImported; // flag for <math.h>
	bool isStdLibImported; // flag for <stdlib.h>
	bool isTimeImported; // flag for <time.h>

	bool hasRndNode; // flag for seeding
	bool needsRoundFunct; // flag for round() creation

public:
	IncludesSubPhase(ostream*);

	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(RandomingNode * n);
	void visit(EndingNode * n){}
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n);
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n);
	void visit(SubingNode * n);
	void visit(MulingNode * n);
	void visit(DivingNode * n);
	void visit(ModDivingNode * n);
	void visit(IDivingNode * n);
	void visit(RootingNode * n);
	void visit(ExpingNode * n);
	void visit(NotingNode * n);
	void visit(LTingNode * n);
	void visit(GTingNode * n);
	void visit(EQingNode * n);
	void visit(LTEingNode * n);
	void visit(GTEingNode * n);

	bool hasRandomNode(){ return hasRndNode; }
	bool needsRoundFunction(){ return needsRoundFunct; }
};