// -----------------------------------------------------------------
// This is the header for the ImportsSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating import
// statements as a subphase of the CodeGenerationPhase.
//
// Version 3.0
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
	bool needsStringToDouble; // flag for stringToDouble() creation
	bool needsStringToAscii; // flag for stringToAscii() creation

	void importString();
	void importIOStream();
	void importMath();
	void importStdLib();
	void importTime();

public:
	IncludesSubPhase(ostream*);

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n) { n->visitAllChildren(this); }
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n) { n->visitAllChildren(this); }
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n) { n->visitAllChildren(this); }
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n);
	void visit(SubingNode * n) { n->visitAllChildren(this); }
	void visit(MulingNode * n) { n->visitAllChildren(this); }
	void visit(DivingNode * n) { n->visitAllChildren(this); }
	void visit(ModDivingNode * n);
	void visit(IDivingNode * n);
	void visit(RootingNode * n);
	void visit(ExpingNode * n);
	void visit(NotingNode * n) { n->visitOperand(this); }
	void visit(LTingNode * n) { n->visitAllChildren(this); }
	void visit(GTingNode * n) { n->visitAllChildren(this); }
	void visit(EQingNode * n) { n->visitAllChildren(this); }
	void visit(LTEingNode * n) { n->visitAllChildren(this); }
	void visit(GTEingNode * n) { n->visitAllChildren(this); }

	bool hasRandomNode(){ return hasRndNode; }
	bool needsRoundFunction(){ return needsRoundFunct; }
	bool needsString(){ return isStringImported; }
	bool needsStringToDoubleFunction() { return needsStringToDouble; }
	bool needsStringToAsciiFunction() { return needsStringToAscii; }
};