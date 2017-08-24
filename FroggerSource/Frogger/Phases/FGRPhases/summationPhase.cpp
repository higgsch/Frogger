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
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::visit(JmpStmtNode * n)
{
	n->visitThisStmt(this);
	n->setJump(n->getStmt()->getAscii());

	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::visit(FunctionCallNode * n)
{
	//Function call adds function name, '(', and ')' to the AST.
	string asciiAddition = n->getLexeme() + "()";

	//Functions with a parent add ':' to the AST (e.g. 65:toString(); )
	if (n->getPrimary() != NULL)
		asciiAddition = ":" + asciiAddition;

	calcAsciiWithAddition(n, asciiAddition);
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::visit(CommandCallNode * n)
{
	//Command call adds command name, '(', and ');' to the AST.
	calcAsciiWithAddition(n, n->getLexeme() + "();");
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::visit(ArgListNode * n)
{
	//Argument list element adds "," to the AST if it is not the first in the list.
	if (n->getArgNo() == 0)
		calcAsciiWithAddition(n, "");
	else
		calcAsciiWithAddition(n, ",");
}

// ----------------------------------------------------------
// This calculates and sets the ascii total of the given node, 
// mods by the number of lines.
// @n: The node to calculate with.
// @addition: The string addition of this node (not including 
//            this node's children).
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::calcAsciiWithAddition(BinaryNode* n, string addition)
{
	n->visitAllChildren(this);

	int ascii = getAsciiSumModLength(addition);
	AsciiNode *left = n->getLeftChild(), *right = n->getRightChild();

	if (left != NULL)
		ascii = accumulateModLength(ascii, left->getAscii());

	if (right != NULL)
		ascii = accumulateModLength(ascii, right->getAscii());
	
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}

// ----------------------------------------------------------
// This calculates and sets the ascii total of the given node, 
// mods by the number of lines.
// @n: The node to calculate with.
// @addition: The string addition of this node.
//
// Version 3.0
// ----------------------------------------------------------
void SummationPhase::calcAsciiWithAddition(TerminalNode* n, string addition)
{
	int ascii = getAsciiSumModLength(addition);
	if (n->getParenNesting() > 0) //only the innermost parens count towards the goto line
		ascii = accumulateModLength(ascii, getAsciiSumModLength("()"));
	n->setAscii(ascii);
}