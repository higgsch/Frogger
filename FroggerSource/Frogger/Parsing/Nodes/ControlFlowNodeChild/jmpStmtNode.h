// -----------------------------------------------------------------
// This is the header file for the StmtNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\controlFlowNode.h"
#include "..\abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node for a statement.
//
// Version 2.0
// ----------------------------------------------------------
class JmpStmtNode : public ControlFlowNode
{
private:
	AbstractNode * stmt; // the AST for this statement
	int ascii_jmp; // the statement to jump to after this statement executes

public:
	JmpStmtNode();

	// ----------------------------------------------------------
	// This function sets the statement to jump to after this 
	// statement executes.
	// @num: The stmtNo to jump to.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setJump(int num) { ascii_jmp = num; }

	// ----------------------------------------------------------
	// This function returns the statement to jump to after this statement
	// executes.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int  getJump() { return ascii_jmp; }

	// ----------------------------------------------------------
	// This function returns the AST for this statement.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AbstractNode * getStmt() { return stmt; }

	// ----------------------------------------------------------
	// This function sets the contents for this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void setStmt(AbstractNode* contents) { stmt = contents; }

	void accept(Phase* p) { p->visit(this); }
	void visitThisStmt(Phase* p)
	{
		if (stmt != NULL)
			stmt->accept(p);
	}
	void visitAllChildren(Phase* p) 
	{
		visitThisStmt(p);
		visitNextStmt(p);
	}

	void addNextStmt(ControlFlowNode* next);
	void clean();
	void printNodes(ostream* out);
};
