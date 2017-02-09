// -----------------------------------------------------------------
// This is the header file for the ProgramNode class. 
// -----------------------------------------------------------------
#pragma once

#include "controlFlowNode.h"
#include "node.h"
using namespace std;

class ControlFlowNode;
class AbstractNode;
class Phase;

// ----------------------------------------------------------
// This class provides a root node for the AST.
//
// Version 2.0
// ----------------------------------------------------------
class ProgramNode : public Node
{
private:
	ControlFlowNode * firstStmt; // the first statement of code in this program
	//int stmtCount; // the total number of statements within the program

public:
	ProgramNode();

	// ----------------------------------------------------------
	// This function returns the first statement of code.
	//
	// Version 2.0
	// ----------------------------------------------------------
	ControlFlowNode* getFirstStmt() { return firstStmt; }

	// ----------------------------------------------------------
	// This function returns the number of statements in this program.
	//
	// Version 1.0
	// ----------------------------------------------------------
	/*int getStmtCount() { return stmtCount; }*/

	void addFirstStmt(ControlFlowNode* first);
	void clean();
	void printNodes(ostream* out);
	void visitAllChildren(Phase* p);

	void accept(Phase* p) { p->visit(this); }
};