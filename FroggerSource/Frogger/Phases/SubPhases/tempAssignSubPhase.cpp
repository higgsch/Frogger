//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for generating temporary's 
// assignment within a given line of code.
// -----------------------------------------------------------------
#include "tempAssignSubPhase.h"
using namespace std;

// ----------------------------------------------------------
// This constructor generates a TempAssignSubPhase for the
// given output stream.
// @outstream: The output stream to print to.
//
// Version 2.5
// ----------------------------------------------------------
TempAssignSubPhase::TempAssignSubPhase(ostream* outstream, int indentCount)
{
	indentDepth = indentCount;
	out = outstream;
	dblTempNo = 1; //temporaries are 1-indexed
	strTempNo = 1;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 3.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(JmpStmtNode * n)
{
	dblTempNo = 1; //restart temporary counters (1-indexed)
	strTempNo = 1;

	n->visitThisStmt(this);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void TempAssignSubPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	Function * funct = n->getFunct();
	if (!funct->isUserDefined())
	{
		string name = funct->name;
		if (name == "retrieveDouble")
		{
			*out << indent() << "cin >> _dbltemp_" << dblTempNo++ << ";\n";
		}
		else if (name == "retrieveString")
		{
			*out << indent() << "cin >> _strtemp_" << strTempNo++ << ";\n";
		}
	}
}