//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a node for a statement.
// -----------------------------------------------------------------
#include "jmpStmtNode.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a statement.
// @stmtNumber: The number identification associated to this
// statement.
//
// Version 2.0
// ----------------------------------------------------------
JmpStmtNode::JmpStmtNode()
{
	stmt = NULL;
	nextStmt = NULL;
	stmtNo = -1;
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
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void JmpStmtNode::clean()
{
	if (stmt != NULL)
	{
		stmt->clean();
		free(stmt);
	}
	if (nextStmt != NULL)
	{
		nextStmt->clean();
		free(nextStmt);
	}
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void JmpStmtNode::printNodes(ostream* out)
{
	if (stmt == NULL)
		*out << "NULL";
	else if (stmtNo != -1) // -1 is a flag for sub-statement statements
	{
		*out << "Stmt " << stmtNo << ": ";
		stmt->printMe(out);
	}
	else
		stmt->printMe(out);

	*out << " Jump to: " << ascii_jmp << endl;

	if (stmtNo != -1)
	{
		if (nextStmt == NULL)
			*out << "NULL";
		else
			nextStmt->printNodes(out);
	}
}