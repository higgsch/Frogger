//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for calculating the goto line
// numbers.
// -----------------------------------------------------------------
#include "summationPhase.h"
using namespace std;

// ----------------------------------------------------------
// This constructor generates a SummationPhase for the
// given number of lines of code.
// @numberOfStmts: The total number of lines in the source.
//
// Version 3.0
// ----------------------------------------------------------
SummationPhase::SummationPhase()
{
	numStmts = 0;
}

// ----------------------------------------------------------
// This function adds the augend and the addend in mod length.
// @augend: The left side of the addition (assumed to already
//          be mod length).
// @addend: The right side of the addition (not mod length).
//
// Version 1.0
// ----------------------------------------------------------
int SummationPhase::accumulateModLength(int augend, int addend)
{
	//overflow "safe" modular arithmetic
	return (augend + (addend % numStmts)) % numStmts;
}

// ----------------------------------------------------------
// This function returns the sum of the ascii values from the
// given string in mod length.
// @s: The string to summate.
//
// Version 1.0
// ----------------------------------------------------------
int SummationPhase::getAsciiSumModLength(string s)
{
	int sum = 0; 
	bool lastWasSpace = false;
	for (char c : s)
	{
		if (c != ' ' && !lastWasSpace)
		{
			sum = accumulateModLength(sum, c);
		}
		else if (c != ' ' && lastWasSpace)
		{
			lastWasSpace = false;
			sum = accumulateModLength(sum, c);
		}
		else if (c == ' ' && !lastWasSpace)
		{
			lastWasSpace = true;
			sum = accumulateModLength(sum, c);
		}
		else //c == ' ' && lastWasSpace
		{
			//No op (ignore subsequent spaces)
		}
	}
	return sum;
}

// ----------------------------------------------------------
// This function initiates the phase over the AST.
// @n: The node representing the program.
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::visit(ProgramNode * n)
{ 
	numStmts = n->getLineCount();

	n->visitAllChildren(this); 
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.0
// ----------------------------------------------------------
void SummationPhase::visit(JmpStmtNode * n)
{
	n->visitThisStmt(this);
	n->setJump(n->getStmt()->getAscii());

	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void SummationPhase::visit(IfNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(IdRefNode * n)
{
	int ascii = getAsciiSumModLength(n->getLexeme());
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void SummationPhase::visit(AssigningNode * n)
{
	n->visitAllChildren(this);

	//Assignment adds '=' and ';' to the AST.
	int ascii = getAsciiSumModLength("=;");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void SummationPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	//Function call adds function name, '(', and ')' to the AST.
	int ascii = getAsciiSumModLength(n->getLexeme() + "()");

	//Functions with a parent add ':' to the AST (e.g. 65:toString(); )
	if (n->getLeftChild() != NULL)
		ascii = accumulateModLength(ascii, getAsciiSumModLength(":"));

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	if (left != NULL)
		ascii = accumulateModLength(ascii, left->getAscii());
	if (right != NULL)
		ascii = accumulateModLength(ascii, right->getAscii());
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void SummationPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	//Command call adds command name, '(', and ')' to the AST.
	int ascii = getAsciiSumModLength(n->getLexeme() + "()");

	//Commands do not have a left child
	AbstractNode *right = n->getRightChild();
	if (right != NULL)
		ascii = accumulateModLength(ascii, right->getAscii());
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 2.4
// ----------------------------------------------------------
void SummationPhase::visit(ArgListNode * n)
{
	n->visitAllChildren(this);

	//Argument list element adds "," to the AST if it is not the first in the list.
	int ascii = getAsciiSumModLength(",");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	if (right != NULL)
		ascii = accumulateModLength(ascii, right->getAscii());
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(StringConstingNode * n)
{
	n->setAscii(getAsciiSumModLength(n->getLexeme()));
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(DoubleConstingNode * n)
{
	int ascii = getAsciiSumModLength(n->getLexeme());
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(AddingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("++");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(SubingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("--");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(MulingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("**");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void SummationPhase::visit(DivingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("//");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void SummationPhase::visit(ModDivingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("%%");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void SummationPhase::visit(IDivingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("\\\\");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void SummationPhase::visit(RootingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("##");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void SummationPhase::visit(ExpingNode * n)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength("^^");
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	ascii = accumulateModLength(ascii, left->getAscii());
	ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}