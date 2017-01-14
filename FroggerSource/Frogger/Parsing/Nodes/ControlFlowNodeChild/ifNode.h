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

	void addNextStmt(ControlFlowNode * next);
	void clean();
	void printNodes(ostream* out);
};