//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a node for an if statement.
// -----------------------------------------------------------------
#include "ifNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
IfNode::IfNode()
{
	boolExp = NULL;
	trueStmt = NULL;
	falseStmt = NULL;
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
IfNode::~IfNode()
{
	if (boolExp != NULL)
		delete boolExp;

	if (trueStmt != NULL)
		delete trueStmt;

	if (falseStmt != NULL)
		delete falseStmt;

	//TODO need to delete nextStmt?
}

// ----------------------------------------------------------
// Adds a stmt to the list of stmts.
// @next:	The stmt to add.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::addNextStmt(ControlFlowNode* next)
{
	if (nextStmt == NULL)
		nextStmt = next;
	else
		nextStmt->addNextStmt(next);
}

// ----------------------------------------------------------
// This function determines if the tree down from this node
// has been successfully typed yet.
//
// Version 3.0
// ----------------------------------------------------------
bool IfNode::isTreeTyped()
{
	if (boolExp != NULL && !boolExp->isTreeTyped())
		return false;

	if (trueStmt != NULL && !trueStmt->isTreeTyped())
		return false;

	if (falseStmt != NULL && !falseStmt->isTreeTyped())
		return false;

	return isTyped();
}