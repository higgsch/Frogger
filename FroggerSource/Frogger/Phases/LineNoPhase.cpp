//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for setting line numbers.
// -----------------------------------------------------------------
#include "lineNoPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function initiates the phase over the AST.
// @n: The node representing the program.
//
// Version 3.0
// ----------------------------------------------------------
void LineNoPhase::visit(ProgramNode * n)
{ 
	n->visitAllChildren(this); 

	n->setLineCount(getLineCount());
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 3.0
// ----------------------------------------------------------
void LineNoPhase::visit(JmpStmtNode * n)
{
	if (n->isNested())
		return;
	
	n->setStmtNo(lineCount++);

	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void LineNoPhase::visit(IfNode * n)
{
	if (n->isNested())
		return;
	
	n->setStmtNo(lineCount++);

	n->visitNextStmt(this);
}