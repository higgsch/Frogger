// -----------------------------------------------------------------
// This is the header for the StringConversionPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for converting string
// literals from frogger syntax to c++ syntax.
//
// Version 2.2
// ----------------------------------------------------------
class StringConversionPhase : public Phase
{
private:
	string convertString(string);

public:
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n){}
	void visit(DisplayingNode * n);
	void visit(RandomingNode * n){}
	void visit(EndingNode * n){}
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n){}
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n){}
	void visit(SubingNode * n){}
	void visit(MulingNode * n){}
	void visit(DivingNode * n){}
	void visit(ModDivingNode * n){}
	void visit(IDivingNode * n){}
	void visit(RootingNode * n){}
	void visit(ExpingNode * n){}
	void visit(NotingNode * n){}
	void visit(LTingNode * n){}
	void visit(GTingNode * n){}
	void visit(EQingNode * n){}
	void visit(LTEingNode * n){}
	void visit(GTEingNode * n){}
};