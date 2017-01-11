//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a node for an if statement.
// -----------------------------------------------------------------
#include "ifNode.h"
#include "stmtNode.h"
#include "..\AbstractNodeChild\binaryOpNode.h"
using namespace std;


// ----------------------------------------------------------
// This constructor builds a node for an if statement.
// @stmtNumber: The number identification associated to this
// statement.
//
// Version 2.0
// ----------------------------------------------------------
IfNode::IfNode(int stmtNumber)
{
	boolExp = NULL;
	trueStmt = NULL;
	falseStmt = NULL;
	nextStmt = NULL;
	stmtNo = stmtNumber;
}

// ----------------------------------------------------------
// Adds a statement's root node to the AST.
// @addStmt: The statement's root node.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::addStmt(AbstractNode * addStmt)
{
	if (boolExp == NULL)
	{ // this IfNode is the final in the list
		ast_error("Empty IF Node - Expected Empty STMT Node");
		return;
	}

	if (nextStmt == NULL) 
		//this IfNode holds an if statement
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
void IfNode::addIf(IfStruct ifStruct)
{
	if (boolExp == NULL)
	{
		boolExp = ifStruct.boolExp;
		trueStmt = ifStruct.trueStmt;
		falseStmt = ifStruct.falseStmt;
		return;
	}

	if (nextStmt == NULL)
		//this IfNode holds an if statement
		//and there is no next ControlFlowNode in the list
		nextStmt = new IfNode(stmtNo + 1);

	nextStmt->addIf(ifStruct);
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

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::traverseNodes(Phase* p)
{
	if (boolExp != NULL)
	{
		p->visit(this);
	}
	if (nextStmt != NULL)
		nextStmt->traverseNodes(p);
}