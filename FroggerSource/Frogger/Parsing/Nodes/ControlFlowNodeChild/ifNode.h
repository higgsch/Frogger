// -----------------------------------------------------------------
// This is the header file for the IfNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\controlFlowNode.h"
//#include "stmtNode.h"
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
	StmtNode * trueStmt; // the statement if boolExp is true
	StmtNode * falseStmt; // the statement if boolExp is false

public: 
	IfNode(int);

	BinaryOpNode* getBoolExp() { return boolExp; }
	StmtNode* getTrueStmt() { return trueStmt; }
	StmtNode* getFalseStmt() { return falseStmt; }

	void addStmt(AbstractNode * stmt);
	void addIf(IfStruct ifStruct);
	void clean();
	void printNodes(ostream* out);
	void traverseNodes(Phase* p);
};