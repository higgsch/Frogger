// -----------------------------------------------------------------
// This is the header file for the ProgramNode class. 
// -----------------------------------------------------------------
#pragma once

#include "ControlFlowNodes\controlFlowNode.h"
#include "node.h"
using namespace std;

class FGRPhase;

// ----------------------------------------------------------
// This class provides a root node for the AST.
//
// Version 3.1
// ----------------------------------------------------------
class ProgramNode : public Node
{
private:
	ControlFlowNode * firstStmt; // the first statement of code in this program
	int lineCount; // the total number of statements within the program

public:
	ProgramNode();
	~ProgramNode();

	ControlFlowNode* getFirstStmt() { return firstStmt; }

	void setLineCount(int i_lineCount) { lineCount = i_lineCount; }
	int getLineCount() { return lineCount; }

	void addFirstStmt(ControlFlowNode* first);

	bool isTreeTyped();

	void visitFirstStmt(FGRPhase* p) { visitNode(p, firstStmt); }
	void visitAllChildren(FGRPhase* p) { visitNode(p, firstStmt); }
	void accept(FGRPhase* p) { p->visit(this); }
};