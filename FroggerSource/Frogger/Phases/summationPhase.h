// -----------------------------------------------------------------
// This is the header for the SummationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for calculating goto line
// numbers.
//
// Version 3.0
// ----------------------------------------------------------
class SummationPhase : public Phase
{
private:
	int numStmts; // the total number of lines in the program

	int getAsciiSumModLength(string);
	int accumulateModLength(int,int);

public:
	SummationPhase();

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
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
	void visit(NotingNode * n){}
	void visit(LTingNode * n){}
	void visit(GTingNode * n){}
	void visit(EQingNode * n){}
	void visit(LTEingNode * n){}
	void visit(GTEingNode * n){}
};