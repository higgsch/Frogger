// -----------------------------------------------------------------
// This is the header for the TempAssignSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating temporary's
// assignments for a given line of code.
//
// Version 1.0
// ----------------------------------------------------------
class TempAssignSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	int tempNo; // the current temp number in a line

public:
	TempAssignSubPhase(ostream* outstream);

	void visit(LineNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(EndingNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n);
	void visit(SubingNode * n);
	void visit(MulingNode * n);
	void visit(DivingNode * n);
};