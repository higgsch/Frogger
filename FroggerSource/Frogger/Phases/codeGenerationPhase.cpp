//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
#include "SubPhases\varDecSubPhase.h"
#include "SubPhases\tempAssignSubPhase.h"
#include "..\Parsing\nodes.h"
#include <iostream>
#include <sstream>
using namespace std;

// ----------------------------------------------------------
// This constructor generates a CodeGenerationPhase for the
// given output stream and AST.
// @outstream: The output stream to print to.
// @root: The root node for the AST.
//
// Version 2.0
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase(ostream* outstream, ProgramNode* root)
{
	out = outstream;
	tempNo = 1; //temporaries are 1-indexed
	indentDepth = 0;

	//emit the "header" code
	*out << "#include <string>\n#include <iostream>\nusing namespace std;\n\n";
	*out << "int main(int argc, char* argv[])\n{\n";
	indentDepth++;

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth);
	root->traverseNodes(sub);
	sub->addTemporaries();
	*out << endl << endl;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(StmtNode * n)
{
	tempNo = 1; //restart temporary counter (1-indexed)

	bool isOwnLine = (n->getStmtNo() != -1);

	if (isOwnLine) 
	{
		//emit this line's label
		*out << indent() << "__LABEL_" << n->getStmtNo() << ":" << endl;
		indentDepth++;
	}

	//generate temp assignments for the line
	n->getStmt()->accept(new TempAssignSubPhase(out, indentDepth)); 

	//emit the line's code
	n->getStmt()->accept(this);
	
	//emit this line's goto statement
	*out << indent() << "goto __LABEL_" << n->getJump() << ";" << endl;

	if (isOwnLine)
	{
		*out << endl;
		indentDepth--;
	}
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IfNode * n)
{
	*out << indent() << "__LABEL_" << n->getStmtNo() << ":" << endl;
	indentDepth++;
	*out << indent() << "if (";
	n->getBoolExp()->accept(this);
	*out << ")\n" << indent() << "{\n";
	indentDepth++;
	visit(n->getTrueStmt());
	indentDepth--;
	*out << indent() << "}\n";
	*out << indent() << "else\n" << indent() << "{\n";
	indentDepth++;
	visit(n->getFalseStmt());
	indentDepth--;
	*out << indent() << "}\n" << endl;
	indentDepth--;
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
	*out << indent() << "cout << (";
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
	*out << indent() << "exit(0);" << endl;
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
	*out << indent();
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

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(NotingNode * n) 
{
	*out << "!( ";
	n->getLeftChild()->accept(this);
	*out << " )";
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTingNode * n) 
{
	n->getLeftChild()->accept(this);
	*out << " < ";
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTingNode * n) 
{
	n->getLeftChild()->accept(this);
	*out << " > ";
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(EQingNode * n) 
{
	n->getLeftChild()->accept(this);
	*out << " == ";
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTEingNode * n) 
{
	n->getLeftChild()->accept(this);
	*out << " <= ";
	n->getRightChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTEingNode * n) 
{
	n->getLeftChild()->accept(this);
	*out << " >= ";
	n->getRightChild()->accept(this);
}