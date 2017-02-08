//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program represents a visitor for converting string literals
// from frogger syntax to c++ syntax.
// -----------------------------------------------------------------
#include "stringConversionPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function converts the given frogger string to c++
// syntax.
// @s: The string to convert.
//
// Version 1.0
// ----------------------------------------------------------
string StringConversionPhase::convertString(string s)
{
	string result = "\"";
	for (unsigned int i = 1; i < s.length() - 1; i++) //First and final char is "\'"
	{
		int c = s[i];
		if (c == '\\') // 'a\nb' is not an escaped newline but a literal slash
			result += "\\\\";
		else if (c != '&')
			result += c;
		else
		{
			c = s[++i];
			if (c != '&') //&& is the only non-escaped translation to c++
				result += "\\";
			result += c;
		}
	}
	result += "\"";
	return result;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.0
// ----------------------------------------------------------
void StringConversionPhase::visit(JmpStmtNode * n)
{
	n->getStmt()->accept(this);

	if (!n->isNested() && n->getNextStmt() != NULL)
		n->getNextStmt()->accept(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.0
// ----------------------------------------------------------
void StringConversionPhase::visit(IfNode * n)
{
	n->getTrueStmt()->accept(this);
	n->getFalseStmt()->accept(this);

	if (!n->isNested() && n->getNextStmt() != NULL)
		n->getNextStmt()->accept(this);
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 1.0
// ----------------------------------------------------------
void StringConversionPhase::visit(DisplayingNode * n)
{
	AbstractNode *child = n->getLeftChild();
	child->accept(this);
}

// ----------------------------------------------------------
// This function processes a string assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void StringConversionPhase::visit(AssigningStringNode * n)
{
	//Don't need to visit leftChild because leftChild is an id
	AbstractNode *right = n->getRightChild();
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void StringConversionPhase::visit(StringConstingNode * n)
{
	n->setLexeme(convertString(n->getLexeme()));
}

// ----------------------------------------------------------
// This function processes a string concatenation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void StringConversionPhase::visit(StringConcatingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a double concatenation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void StringConversionPhase::visit(DoubleConcatingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an ascii concatenation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void StringConversionPhase::visit(AsciiConcatingNode * n)
{
	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	left->accept(this);
	right->accept(this);
}