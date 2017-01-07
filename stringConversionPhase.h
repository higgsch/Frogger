// -----------------------------------------------------------------
// This is the header for the StringConversionPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for converting string
// literals from frogger syntax to c++ syntax.
//
// Version 1.1
// ----------------------------------------------------------
class StringConversionPhase : public Phase
{
private:
	string convertString(string);

public:
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