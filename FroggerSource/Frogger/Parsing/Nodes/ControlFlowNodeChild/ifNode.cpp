//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a node for an if statement.
// -----------------------------------------------------------------
#include "ifNode.h"
#include "jmpStmtNode.h"
#include "..\AbstractNodeChild\binaryOpNode.h"
using namespace std;


// ----------------------------------------------------------
// This constructor builds a node for an if statement.
// @stmtNumber: The number identification associated to this
// statement.
//
// Version 2.0
// ----------------------------------------------------------
IfNode::IfNode()
{
	boolExp = NULL;
	trueStmt = NULL;
	falseStmt = NULL;
	nextStmt = NULL;
	stmtNo = -1;
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
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::clean()
{
	if (boolExp != NULL)
	{
		boolExp->clean();
		free(boolExp);
	}

	if (trueStmt != NULL)
	{
		trueStmt->clean();
		free(trueStmt);
	}

	if (falseStmt != NULL)
	{
		falseStmt->clean();
		free(falseStmt);
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
void IfNode::printNodes(ostream* out)
{
	if (boolExp == NULL)
		*out << "NULL";
	else
	{
		*out << "if ";
		boolExp->printMe(out);
		*out << "\n\t";
		trueStmt->printNodes(out);
		*out << " else\n\t";
		falseStmt->printNodes(out);
	}

	if (nextStmt == NULL)
		*out << "NULL";
	else
		nextStmt->printNodes(out);
}