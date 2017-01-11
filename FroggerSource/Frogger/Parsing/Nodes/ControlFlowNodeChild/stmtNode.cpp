//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a node for a statement.
// -----------------------------------------------------------------
#include "..\abstractNode.h"
#include "stmtNode.h"
#include "ifNode.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a statement.
// @stmtNumber: The number identification associated to this
// statement.
//
// Version 2.0
// ----------------------------------------------------------
StmtNode::StmtNode(int stmtNumber)
{
	stmt = NULL;
	nextStmt = NULL;
	stmtNo = stmtNumber;
}

// ----------------------------------------------------------
// Adds a statement's root node to the AST.
// @addStmt: The statement's root node.
//
// Version 2.0
// ----------------------------------------------------------
void StmtNode::addStmt(AbstractNode * addStmt)
{
	if (stmt == NULL)
	{ // this StmtNode is the final in the list
		stmt = addStmt;
		return;
	}

	if (nextStmt == NULL) 
		//this StmtNode holds a statement
		//and there is no next ControlFlowNode in the list
		nextStmt = new StmtNode(stmtNo + 1);

	nextStmt->addStmt(addStmt);
}

// ----------------------------------------------------------
// Adds an if statement's root node to the AST.
// @ifStruct:	The if summary.
//
// Version 2.0
// ----------------------------------------------------------
void StmtNode::addIf(IfStruct ifStruct)
{
	if (stmt == NULL)
	{
		ast_error("Empty STMT Node - Expected Empty IF Node");
		return;
	}

	if (nextStmt == NULL)
		//this StmtNode holds a statement
		//and there is no next ControlFlowNode in the list
		nextStmt = new IfNode(stmtNo + 1);

	nextStmt->addIf(ifStruct);
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void StmtNode::clean()
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
void StmtNode::printNodes(ostream* out)
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

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void StmtNode::traverseNodes(Phase* p)
{
	if (stmt != NULL)
	{
		p->visit(this);
	}
	if (stmtNo != -1 && nextStmt != NULL)
		nextStmt->traverseNodes(p);
}