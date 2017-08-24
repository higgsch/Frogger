// -----------------------------------------------------------------
// This is the header for the SummationPhase class.
// -----------------------------------------------------------------
#pragma once

#include "fgrPhase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for calculating goto line
// numbers.
//
// Version 3.0
// ----------------------------------------------------------
class SummationPhase : public FGRPhase
{
private:
	int numStmts; // the total number of lines in the program

	int getAsciiSumModLength(string);
	int accumulateModLength(int,int);

	void calcAsciiWithAddition(BinaryNode* n, string addition);
	void calcAsciiWithAddition(TerminalNode* n, string addition);

public:
	SummationPhase() { numStmts = 0; }

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n) { calcAsciiWithAddition(n, n->getLexeme()); }
	void visit(AssigningNode * n) { calcAsciiWithAddition(n, "=;"); }
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n) { calcAsciiWithAddition(n, n->getLexeme()); }
	void visit(DoubleConstingNode * n) { calcAsciiWithAddition(n, n->getLexeme()); }
	void visit(AddingNode * n) { calcAsciiWithAddition(n, "++"); }
	void visit(SubingNode * n) { calcAsciiWithAddition(n, "--"); }
	void visit(MulingNode * n) { calcAsciiWithAddition(n, "**"); }
	void visit(DivingNode * n) { calcAsciiWithAddition(n, "//"); }
	void visit(ModDivingNode * n) { calcAsciiWithAddition(n, "%%"); }
	void visit(IDivingNode * n) { calcAsciiWithAddition(n, "\\\\"); }
	void visit(RootingNode * n) { calcAsciiWithAddition(n, "##"); }
	void visit(ExpingNode * n) { calcAsciiWithAddition(n, "^^"); }
	void visit(NotingNode * n){}
	void visit(LTingNode * n){}
	void visit(GTingNode * n){}
	void visit(EQingNode * n){}
	void visit(LTEingNode * n){}
	void visit(GTEingNode * n){}
};