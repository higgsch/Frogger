// -----------------------------------------------------------------
// This is the header for the FGRStringConversionPhase class.
// -----------------------------------------------------------------
#pragma once

#include "fgrPhase.h"
#include "..\stringConverter.h"
#include "..\..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for converting string
// literals from frogger syntax to c++ syntax.
//
// Version 5.0
// ----------------------------------------------------------
class FGRStringConversionPhase : public FGRPhase
{
private:
	StringConverter converter;

public:
	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n) { n->visitAllChildren(this); }
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n) { n->visitAllChildren(this); }
	void visit(FunctionCallNode * n) { n->visitAllChildren(this); }
	void visit(CommandCallNode * n) { n->visitAllChildren(this); }
	void visit(ArgListNode * n) { n->visitAllChildren(this); }
	void visit(StringConstingNode * n) { n->setLexeme(converter.convertString(n->getLexeme())); }
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n) { n->visitAllChildren(this); }
	void visit(SubingNode * n){ n->visitAllChildren(this); }
	void visit(MulingNode * n){ n->visitAllChildren(this); }
	void visit(DivingNode * n){ n->visitAllChildren(this); }
	void visit(ModDivingNode * n){ n->visitAllChildren(this); }
	void visit(IDivingNode * n){ n->visitAllChildren(this); }
	void visit(RootingNode * n){ n->visitAllChildren(this); }
	void visit(ExpingNode * n){ n->visitAllChildren(this); }
	void visit(NotingNode * n){ n->visitAllChildren(this); }
	void visit(LTingNode * n){ n->visitAllChildren(this); }
	void visit(GTingNode * n){ n->visitAllChildren(this); }
	void visit(EQingNode * n){ n->visitAllChildren(this); }
	void visit(LTEingNode * n){ n->visitAllChildren(this); }
	void visit(GTEingNode * n){ n->visitAllChildren(this); }
};