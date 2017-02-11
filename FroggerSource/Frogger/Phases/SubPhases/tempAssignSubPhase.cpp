//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.5
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
// Version 2.5
// ----------------------------------------------------------
void TempAssignSubPhase::visit(JmpStmtNode * n)
{
	dblTempNo = 1; //restart temporary counters (1-indexed)
	strTempNo = 1;

	//emit the line's code
	n->visitThisStmt(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(IfNode * n)
{
	n->visitBoolExp(this);
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(AssigningNode * n)
{
	//Ignore left as left cannot be a temporary
	n->visitRightChild(this);
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

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void TempAssignSubPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 2.4
// ----------------------------------------------------------
void TempAssignSubPhase::visit(ArgListNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(AddingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(SubingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(MulingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(DivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void TempAssignSubPhase::visit(ModDivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void TempAssignSubPhase::visit(IDivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void TempAssignSubPhase::visit(RootingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void TempAssignSubPhase::visit(ExpingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(NotingNode * n) 
{
	n->visitLeftChild(this);
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(LTingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(GTingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(EQingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(LTEingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(GTEingNode * n) 
{
	n->visitAllChildren(this);
}