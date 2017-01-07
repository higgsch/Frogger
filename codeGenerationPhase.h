// -----------------------------------------------------------------
// This is the header for the CodeGeneratingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating output code
// that reflects the current AST.
//
// Version 1.1
// ----------------------------------------------------------
class CodeGenerationPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	int tempNo; // the current temp number in a line

public:
	CodeGenerationPhase(ostream* outstream, ProgramNode* root);

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