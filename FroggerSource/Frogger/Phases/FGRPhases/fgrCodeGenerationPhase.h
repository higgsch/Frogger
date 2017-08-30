// -----------------------------------------------------------------
// This is the header for the FGRCodeGeneratingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "fgrPhase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
#include "..\..\DataStructures\Tables\tables.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating output code
// that reflects the current AST.
//
// Version 5.0
// ----------------------------------------------------------
class FGRCodeGenerationPhase : public FGRPhase
{
private:
	Language * lang;
	string currUDFName;

	void visitBinaryOpNode(BinaryOpNode * n);

	bool isNested(BinaryOpNode * n) { return n->getParenNesting() > 0; }
	string leftText(BinaryOpNode * n) { return n->getLeftOperand()->outputText; }
	string rightText(BinaryOpNode * n) { return n->getRightOperand()->outputText; }

	bool validBuiltInFunctionName(string name);
	bool validBuiltInCommandName(string name);
public:
	FGRCodeGenerationPhase(Language * lang) : lang(lang), currUDFName("<META>") {}

	void updateCurrentUDFName(string newUDFName) { currUDFName = newUDFName; }

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n) { n->outputText = lang->getStringLiteralText(n->getLexeme()); }
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n) { visitBinaryOpNode(n); }
	void visit(SubingNode * n) { visitBinaryOpNode(n); }
	void visit(MulingNode * n) { visitBinaryOpNode(n); }
	void visit(DivingNode * n) { visitBinaryOpNode(n); }
	void visit(ModDivingNode * n) { visitBinaryOpNode(n); }
	void visit(IDivingNode * n) { visitBinaryOpNode(n); }
	void visit(RootingNode * n) { visitBinaryOpNode(n); }
	void visit(ExpingNode * n) { visitBinaryOpNode(n); }
	void visit(NotingNode * n);
	void visit(LTingNode * n) { visitBinaryOpNode(n); }
	void visit(GTingNode * n) { visitBinaryOpNode(n); }
	void visit(EQingNode * n) { visitBinaryOpNode(n); }
	void visit(LTEingNode * n) { visitBinaryOpNode(n); }
	void visit(GTEingNode * n) { visitBinaryOpNode(n); }
};