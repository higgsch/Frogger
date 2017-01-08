//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program represents a visitor for generating temporary's 
// assignment within a given line of code.
// -----------------------------------------------------------------
#include "tempAssignSubPhase.h"
#include "nodes.h"
#include <iostream>
#include <sstream>
using namespace std;

// ----------------------------------------------------------
// This constructor generates a TempAssignSubPhase for the
// given output stream.
// @outstream: The output stream to print to.
//
// Version 1.0
// ----------------------------------------------------------
TempAssignSubPhase::TempAssignSubPhase(ostream* outstream)
{
	out = outstream;
	tempNo = 1; //temporaries are 1-indexed
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(LineNode * n)
{
	tempNo = 1; //restart temporary counter (1-indexed)

	//emit the line's code
	n->getLine()->accept(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(IfNode * n)
{
	visit(n->getTrueLine());
	visit(n->getFalseLine());
}

// ----------------------------------------------------------
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(RetrievalNode * n)
{
	*out << "\t\tcin >> _temp_" << tempNo++ << ";\n";
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(DisplayingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes an end statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(EndingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(IdRefNode * n)
{
	//No op
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
	AbstractNode *right = n->getRightChild();
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(StringConstingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(DoubleConstingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(AddingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(SubingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(MulingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(DivingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(NotingNode * n) 
{
	//No op
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(LTingNode * n) 
{
	//No op
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(GTingNode * n) 
{
	//No op
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void TempAssignSubPhase::visit(EQingNode * n) 
{
	//No op
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
	//No op
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
	//No op
}