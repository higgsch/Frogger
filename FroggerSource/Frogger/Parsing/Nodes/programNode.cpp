//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a root node for the AST.
// -----------------------------------------------------------------
#include "programNode.h"
#include "controlFlowNode.h"
#include "ControlFlowNodeChild\ifNode.h"
#include "ControlFlowNodeChild\stmtNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
ProgramNode::ProgramNode()
{
	firstStmt = NULL;
	stmtCount = 0;
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::clean()
{
	if (firstStmt != NULL)
	{
		firstStmt->clean();
		free(firstStmt);
	}
}

// ----------------------------------------------------------
// Adds a statement's root node to the AST.
// @stmt: The statement's root node.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::addStmtNode(AbstractNode * stmt)
{
	if (firstStmt == NULL)
		firstStmt = new StmtNode(0);

	firstStmt->addStmt(stmt);

	stmtCount++;
}

// ----------------------------------------------------------
// Adds an if statement's root node to the AST.
// @ifStruct: The statement's root node.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::addIfNode(IfStruct ifStruct)
{
	if (firstStmt == NULL)
		firstStmt = new IfNode(0);

	firstStmt->addIf(ifStruct);

	stmtCount++;
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::printNodes(ostream* out)
{
	cout << "Printing in Order" << endl;
	if (firstStmt == NULL)
		*out << "NULL";
	else
		firstStmt->printNodes(out);

	*out << endl << endl;
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::traverseNodes(Phase* p)
{
	if (firstStmt != NULL)
		firstStmt->traverseNodes(p);
}