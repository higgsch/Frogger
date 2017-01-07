//                      Christopher Higgs
//                      CS 6820 - 7:30 am
//                      Final Project
//                      Dr. Rague
//                      Due: 12/10/16
//                      Version: 1.1
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
#include "varDecSubPhase.h"
#include "tempAssignSubPhase.h"
#include "nodes.h"
#include <iostream>
#include <sstream>
using namespace std;

// ----------------------------------------------------------
// This constructor generates a CodeGenerationPhase for the
// given output stream and AST.
// @outstream: The output stream to print to.
// @root: The root node for the AST.
//
// Version 1.1
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase(ostream* outstream, ProgramNode* root)
{
	out = outstream;
	tempNo = 1; //temporaries are 1-indexed

	//emit the "header" code
	*out << "#include <string>\n#include <iostream>\nusing namespace std;\n\n";
	*out << "int main(int argc, char* argv[])\n{\n";

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out);
	root->traverseNodes(sub);
	sub->addTemporaries();
	*out << endl << endl;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 1.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LineNode * n)
{
	tempNo = 1; //restart temporary counter (1-indexed)

	//emit this line's label
	*out << "\t__LABEL_" << n->getLineNo() << ":" << endl;

	//generate temp assignments for the line
	n->getLine()->accept(new TempAssignSubPhase(out)); 

	//emit the line's code
	n->getLine()->accept(this);

	//emit this line's goto statement
	*out << "\t\tgoto __LABEL_" << n->getJump() << ";" << endl << endl;
}

// ----------------------------------------------------------
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 1.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(RetrievalNode * n)
{
	*out << " _temp_" << tempNo++ << " ";
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DisplayingNode * n)
{
	*out << "\t\tcout << (";
	AbstractNode *child = n->getLeftChild();
	child->accept(this);
	*out << ");" << endl;
}

// ----------------------------------------------------------
// This function processes an end statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(EndingNode * n)
{
	*out << "\t\texit(0);" << endl;
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IdRefNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	//prepend identifiers to avoid c++ keyword conflicts
	*out << "_" << n->getLexeme();
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AssigningNode * n)
{
	*out << "\t\t";
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	*out << " = (";
	right->accept(this);
	*out << ");" << endl;
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(StringConstingNode * n)
{
	*out << n->getLexeme();
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DoubleConstingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << n->getLexeme();
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AddingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	*out << " + ";
	right->accept(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(SubingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	*out << " - ";
	right->accept(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(MulingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	*out << " * ";
	right->accept(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	*out << " / ";
	right->accept(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}
