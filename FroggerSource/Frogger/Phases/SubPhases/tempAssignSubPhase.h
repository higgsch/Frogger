// -----------------------------------------------------------------
// This is the header for the TempAssignSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phases.h"
#include "..\..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating temporary's
// assignments for a given line of code.
//
// Version 2.3
// ----------------------------------------------------------
class TempAssignSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	int tempNo; // the current temp number in a line
	int indentDepth; // the number of tabs to insert

public:
	TempAssignSubPhase(ostream* outstream, int indentCount);

	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(RandomingNode * n){} 
	void visit(EndingNode * n){}
	void visit(IdRefNode * n){}
	void visit(AssigningDoubleNode * n);
	void visit(AssigningStringNode * n);
	void visit(StringConstingNode * n){}
	void visit(DoubleConstingNode * n){}
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
	void visit(NotingNode * n);
	void visit(LTingNode * n);
	void visit(GTingNode * n);
	void visit(EQingNode * n);
	void visit(LTEingNode * n);
	void visit(GTEingNode * n);

	string indent()
	{
		string result = "";
		for (int i = 0; i < indentDepth; i++)
			result = result + "\t";

		return result;
	}
};