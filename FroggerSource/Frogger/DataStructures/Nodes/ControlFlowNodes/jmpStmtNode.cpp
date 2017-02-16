//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides a node for a statement.
// -----------------------------------------------------------------
#include "jmpStmtNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
JmpStmtNode::JmpStmtNode()
{
	stmt = NULL;
	ascii_jmp = -1;
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
JmpStmtNode::~JmpStmtNode()
{
	if (stmt != NULL)
		delete stmt;
	
	//TODO need to delete nextStmt?
}

// ----------------------------------------------------------
// Adds a stmt to the list of statements.
// @next:	The stmt to add.
//
// Version 2.0
// ----------------------------------------------------------
void JmpStmtNode::addNextStmt(ControlFlowNode * next)
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
bool JmpStmtNode::isTreeTyped()
{
	if (stmt != NULL && !stmt->isTreeTyped())
		return false;

	if (nextStmt != NULL && !nextStmt->isTreeTyped())
		return false;

	return isTyped();
}