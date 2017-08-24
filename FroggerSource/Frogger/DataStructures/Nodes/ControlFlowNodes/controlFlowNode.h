//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a node for a control flow structure.
// -----------------------------------------------------------------
#pragma once

#include "..\node.h"
using namespace std;

class Phase;

class BinaryOpNode;
class JmpStmtNode;

// ----------------------------------------------------------
// This class provides a node for a control flow structure.
//
// Version 3.0
// ----------------------------------------------------------
class ControlFlowNode : public Node
{
protected: 
	ControlFlowNode * nextStmt; // the next statement linearly
	int stmtNo; // an identifier for this statement 
	bool isNestedStmt; // an indicator for nested statements
		// if true, nextStmt should be NULL and stmtNo is N/A.

public: 
	ControlFlowNode() { nextStmt = NULL; stmtNo = -1; isNestedStmt = false; }
	~ControlFlowNode() { if (nextStmt != NULL) delete nextStmt; }

	ControlFlowNode * getNextStmt() { return (isNestedStmt) ? NULL : nextStmt; }

	int	 getStmtNo() { return (isNestedStmt) ? -1 : stmtNo; }
	void setStmtNo(int num) { stmtNo = num; }

	bool isNested() { return isNestedStmt; }
	void setNested(bool nest) { isNestedStmt = nest; }

	void visitNextStmt(FGRPhase* p) { if (!isNested()) visitNode(p, nextStmt); }

	virtual void addNextStmt(ControlFlowNode* next)=0;
};