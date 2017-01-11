// -----------------------------------------------------------------
// This is the header file for the StmtNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\controlFlowNode.h"
//#include "ifNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node for a statement.
//
// Version 2.0
// ----------------------------------------------------------
class StmtNode : public ControlFlowNode
{
private:
	AbstractNode * stmt; // the AST for this statement
	int ascii_jmp; // the statement to jump to after this statement executes

public:
	StmtNode(int);

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
	
	void addStmt(AbstractNode * stmt);
	void addIf(IfStruct ifStruct);
	void clean();
	void printNodes(ostream* out);
	void traverseNodes(Phase* p);
};
