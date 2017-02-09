//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program represents a visitor for setting line numbers.
// -----------------------------------------------------------------
#include "LineNoPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.0
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
// Version 2.0
// ----------------------------------------------------------
void LineNoPhase::visit(IfNode * n)
{
	if (n->isNested())
		return;
	
	n->setStmtNo(lineCount++);

	n->visitNextStmt(this);
}