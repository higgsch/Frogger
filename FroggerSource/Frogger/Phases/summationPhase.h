// -----------------------------------------------------------------
// This is the header for the SummationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for calculating goto line
// numbers.
//
// Version 2.3
// ----------------------------------------------------------
class SummationPhase : public Phase
{
private:
	int numStmts; // the total number of lines in the program

	int getAsciiSumModLength(string);
	int accumulateModLength(int,int);

public:
	SummationPhase(int);

	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(RandomingNode * n);
	void visit(EndingNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningDoubleNode * n);
	void visit(AssigningStringNode * n);
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n);
	void visit(SubingNode * n);
	void visit(MulingNode * n);
	void visit(DivingNode * n);
	void visit(ModDivingNode * n);
	void visit(IDivingNode * n);
	void visit(RootingNode * n);
	void visit(ExpingNode * n);
	void visit(StringConcatingNode * n);
	void visit(DoubleConcatingNode * n);
	void visit(AsciiConcatingNode * n);
	void visit(NotingNode * n){}
	void visit(LTingNode * n){}
	void visit(GTingNode * n){}
	void visit(EQingNode * n){}
	void visit(LTEingNode * n){}
	void visit(GTEingNode * n){}
};