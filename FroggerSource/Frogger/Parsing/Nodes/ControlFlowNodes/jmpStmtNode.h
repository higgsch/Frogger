// -----------------------------------------------------------------
// This is the header file for the StmtNode class. 
// -----------------------------------------------------------------
#pragma once

#include "controlFlowNode.h"
#include "..\AsciiNodes\asciiNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node for a statement.
//
// Version 3.0
// ----------------------------------------------------------
class JmpStmtNode : public ControlFlowNode
{
private:
	AsciiNode * stmt; // the AST for this statement
	int ascii_jmp; // the statement to jump to after this statement executes

public:
	JmpStmtNode();
	~JmpStmtNode();

	AsciiNode * getStmt() { return stmt; }
	void setStmt(AsciiNode* contents) { stmt = contents; }

	void setJump(int num) { ascii_jmp = num; }
	int  getJump() { return ascii_jmp; }

	void addNextStmt(ControlFlowNode* next);

	bool isTreeTyped();

	void visitThisStmt(Phase* p) { visitNode(p, stmt); }
	void visitAllChildren(Phase* p) { visitThisStmt(p); visitNextStmt(p); }
	void accept(Phase* p) { p->visit(this); }
};
