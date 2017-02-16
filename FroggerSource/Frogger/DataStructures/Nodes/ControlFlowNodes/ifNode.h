// -----------------------------------------------------------------
// This is the header file for the IfNode class. 
// -----------------------------------------------------------------
#pragma once

#include "controlFlowNode.h"
#include "..\AsciiNodes\BinaryNodes\BinaryOpNodes\binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node for an if statement.
//
// Version 3.0
// ----------------------------------------------------------
class IfNode : public ControlFlowNode
{
private: 
	AsciiNode * boolExp; // the boolean comparison
	ControlFlowNode * trueStmt; // the statement if boolExp is true
	ControlFlowNode * falseStmt; // the statement if boolExp is false

public: 
	IfNode();
	~IfNode();

	AsciiNode* getBoolExp() { return boolExp; }
	ControlFlowNode* getTrueStmt() { return trueStmt; }
	ControlFlowNode* getFalseStmt() { return falseStmt; }
	void setBoolExp(AsciiNode* boolExpression) { boolExp = boolExpression; }
	void setTrueStmt(ControlFlowNode* tStmt) { trueStmt = tStmt; }
	void setFalseStmt(ControlFlowNode* fStmt) { falseStmt = fStmt; }

	void addNextStmt(ControlFlowNode * next);

	bool isTreeTyped();

	void visitBoolExp(Phase* p) { visitNode(p, boolExp); }
	void visitTrueStmt(Phase* p) { visitNode(p, trueStmt); }
	void visitFalseStmt(Phase* p) { visitNode(p, falseStmt); }
	void visitThisStmt(Phase* p)
	{
		visitBoolExp(p);
		visitTrueStmt(p);
		visitFalseStmt(p);
	}
	void visitAllChildren(Phase* p) { visitThisStmt(p); visitNextStmt(p); }
	void accept(Phase* p) { p->visit(this); }
};