//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a root node for the AST.
// -----------------------------------------------------------------
#include "programNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
ProgramNode::ProgramNode()
{
	firstStmt = NULL;
	lineCount = 0;
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
ProgramNode::~ProgramNode()
{
	if (firstStmt != NULL)
		delete firstStmt;
}

// ----------------------------------------------------------
// Adds a first statement to the root node.
// @first: The first statement.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::addFirstStmt(ControlFlowNode* first)
{
	if (firstStmt == NULL)
		firstStmt = first;
	else
		ast_error("ProgramNode's root already exists");
}

// ----------------------------------------------------------
// This function determines if the tree down from this node
// has been successfully typed yet.
//
// Version 3.0
// ----------------------------------------------------------
bool ProgramNode::isTreeTyped()
{
	if (firstStmt != NULL && !firstStmt->isTreeTyped())
		return false;

	return isTyped();
}