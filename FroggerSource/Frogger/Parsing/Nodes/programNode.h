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
// Version 3.0
// ----------------------------------------------------------
class ProgramNode : public Node
{
private:
	ControlFlowNode * firstStmt; // the first statement of code in this program
	int lineCount; // the total number of statements within the program

public:
	ProgramNode();

	// ----------------------------------------------------------
	// This function returns the first statement of code.
	//
	// Version 2.0
	// ----------------------------------------------------------
	ControlFlowNode* getFirstStmt() { return firstStmt; }

	void setLineCount(int i_lineCount) { lineCount = i_lineCount; }
	int getLineCount() { return lineCount; }

	void addFirstStmt(ControlFlowNode* first);
	void clean();
	void printNodes(ostream* out);
	void visitAllChildren(Phase* p);

	void accept(Phase* p) { p->visit(this); }
};