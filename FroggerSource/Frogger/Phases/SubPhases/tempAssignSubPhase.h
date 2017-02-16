// -----------------------------------------------------------------
// This is the header for the TempAssignSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating temporary's
// assignments for a given line of code.
//
// Version 3.0
// ----------------------------------------------------------
class TempAssignSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	int dblTempNo; // the current double temp number in a line
	int strTempNo; // the current string temp number in a line
	int indentDepth; // the number of tabs to insert

public:
	TempAssignSubPhase(ostream* outstream, int indentCount);

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
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
	void visit(NotingNode * n);
	void visit(LTingNode * n);
	void visit(GTingNode * n);
	void visit(EQingNode * n);
	void visit(LTEingNode * n);
	void visit(GTEingNode * n);

	// ----------------------------------------------------------
	// This function returns a string containing the current tab
	// indentation.
	//
	// Version 3.0
	// ----------------------------------------------------------
	string indent()
	{
		string result = "";
		for (int i = 0; i < indentDepth; i++)
			result = result + "\t";

		return result;
	}
};