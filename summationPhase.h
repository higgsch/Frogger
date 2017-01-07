// -----------------------------------------------------------------
// This is the header for the SummationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for calculating goto line
// numbers.
//
// Version 1.1
// ----------------------------------------------------------
class SummationPhase : public Phase
{
private:
	int numLines; // the total number of lines in the program

	int getAsciiSumModLength(string);
	int accumulateModLength(int,int);

public:
	SummationPhase(int);

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