//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program represents a visitor for generating variable
// declarations as a subphase of the CodeGenerationPhase.
// -----------------------------------------------------------------
#include "varDecSubPhase.h"
#include "nodes.h"
#include <iostream>
#include <sstream>
using namespace std;

// ----------------------------------------------------------
// This constructor generates a VarDecSubPhase for the
// given output stream.
// @outstream: The output stream to print to.
//
// Version 1.1
// ----------------------------------------------------------
VarDecSubPhase::VarDecSubPhase(ostream* outstream)
{
	list = NULL;
	out = outstream;
	currLineTempCount = 0;
	lineTempMax = 0;
}

// ----------------------------------------------------------
// This function adds the given variable to the symbol table.
// @id: The variable to add.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::addToList(string id)
{
	if (list == NULL)
	{ //this is the first variable in the list
		list = new VarList();
		list->id = id;
		list->tail = NULL;
		return;
	}
	
	//navigate to the last variable in the list
	VarList* curr = list;
	while (curr->tail != NULL)
		curr = curr->tail;

	//add the variable
	curr->tail = new VarList();
	VarList* currtail = curr->tail;
	currtail->id = id;
	currtail->tail = NULL;
}

// ----------------------------------------------------------
// This function returns if the given variable is in the 
// symbol table.
// @id: The variable to check for.
//
// Version 1.0
// ----------------------------------------------------------
bool VarDecSubPhase::isInList(string id)
{
	if (list == NULL)
		return false;

	//traverse the list
	VarList* curr = list;
	while (curr->tail != NULL)
	{
		if (!curr->id.compare(id))
			return true;
		curr = curr->tail;
	}

	if (!curr->id.compare(id))
		return true;
	return false;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 1.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(LineNode * n)
{
	currLineTempCount = 0;

	n->getLine()->accept(this);

	//update the temporary counter
	if (currLineTempCount > lineTempMax)
		lineTempMax = currLineTempCount;
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(IfNode * n)
{
	n->getBoolExp()->accept(this);
	visit(n->getTrueLine());
	visit(n->getFalseLine());
}

// ----------------------------------------------------------
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 1.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(RetrievalNode * n)
{
	currLineTempCount++;
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(DisplayingNode * n)
{
	AbstractNode *child = n->getLeftChild();
	child->accept(this);
}

// ----------------------------------------------------------
// This function processes an end statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(EndingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	if (!isInList(id))
	{
		addToList(id);
		//emit variable declaration and initialization
		*out << "\tdouble _" << id << " = 0;\n";
	}
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(AssigningNode * n)
{
	AbstractNode* left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(StringConstingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(DoubleConstingNode * n)
{
	//No op
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(AddingNode * n)
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
void VarDecSubPhase::visit(SubingNode * n)
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
void VarDecSubPhase::visit(MulingNode * n)
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
void VarDecSubPhase::visit(DivingNode * n)
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
void VarDecSubPhase::visit(NotingNode * n) 
{
	n->getLeftChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(LTingNode * n) 
{
	n->getLeftChild()->accept(this);
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(GTingNode * n) 
{
	n->getLeftChild()->accept(this);
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(EQingNode * n) 
{
	n->getLeftChild()->accept(this);
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(LTEingNode * n) 
{
	n->getLeftChild()->accept(this);
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(GTEingNode * n) 
{
	n->getLeftChild()->accept(this);
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function adds declarations for the temporaries.
//
// Version 1.1
// ----------------------------------------------------------
void VarDecSubPhase::addTemporaries()
{
	for (int i = 1; i <= lineTempMax; i++)
	{
		*out << "\tdouble _temp_" << i << " = 0;\n";
	}
}