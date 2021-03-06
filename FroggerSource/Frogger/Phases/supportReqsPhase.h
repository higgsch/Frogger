// -----------------------------------------------------------------
// This is the header for the SupportReqsPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
#include "..\DataStructures\Tables\tables.h"
#include "..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for gathering support code
// requirements.
//
// Version 4.2
// ----------------------------------------------------------
class SupportReqsPhase : public Phase
{
private:
	Language * lang;
public:
	void gatherRequirements(Language * language, ProgramStruct * prog);

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n) { n->visitAllChildren(this); }
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n);
	void visit(AssigningNode * n) { n->visitAllChildren(this); }
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n) { n->visitAllChildren(this); }
	void visit(StringConstingNode * n){}
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
};