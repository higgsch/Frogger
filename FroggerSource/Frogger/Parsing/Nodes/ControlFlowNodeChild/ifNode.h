// -----------------------------------------------------------------
// This is the header file for the IfNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\controlFlowNode.h"
#include "..\AbstractNodeChild\binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node for an if statement.
//
// Version 2.0
// ----------------------------------------------------------
class IfNode : public ControlFlowNode
{
private: 
	BinaryOpNode * boolExp; // the boolean comparison
	ControlFlowNode * trueStmt; // the statement if boolExp is true
	ControlFlowNode * falseStmt; // the statement if boolExp is false

public: 
	IfNode();

	BinaryOpNode* getBoolExp() { return boolExp; }
	ControlFlowNode* getTrueStmt() { return trueStmt; }
	ControlFlowNode* getFalseStmt() { return falseStmt; }
	void setBoolExp(BinaryOpNode* boolExpression) { boolExp = boolExpression; }
	void setTrueStmt(ControlFlowNode* tStmt) { trueStmt = tStmt; }
	void setFalseStmt(ControlFlowNode* fStmt) { falseStmt = fStmt; }

	void accept(Phase* p) { p->visit(this); }
	void visitBoolExp(Phase* p)
	{
		if (boolExp != NULL)
			boolExp->accept(p);
	}
	void visitTrueStmt(Phase* p)
	{
		if (trueStmt != NULL)
			trueStmt->accept(p);
	}
	void visitFalseStmt(Phase* p)
	{
		if (falseStmt != NULL)
			falseStmt->accept(p);
	}
	void visitThisStmt(Phase* p)
	{
		visitBoolExp(p);
		visitTrueStmt(p);
		visitFalseStmt(p);
	}
	void visitAllChildren(Phase* p)
	{
		visitThisStmt(p);
		visitNextStmt(p);
	}

	void addNextStmt(ControlFlowNode * next);
	void clean();
	void printNodes(ostream* out);
};