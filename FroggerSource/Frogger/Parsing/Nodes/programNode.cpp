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
void ProgramNode::visitAllChildren(Phase* p)
{
	if (firstStmt != NULL)
		firstStmt->accept(p);
}