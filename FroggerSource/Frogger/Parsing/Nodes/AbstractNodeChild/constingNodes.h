// -----------------------------------------------------------------
// This is the header file for the consting classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a string
// constant.
//
// Version 1.0
// ----------------------------------------------------------
class StringConstingNode : public AbstractNode
{
public:
	StringConstingNode(string);

	// ----------------------------------------------------------
	// This function prints this node to the given output stream.
	// @out: The stream to display to.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void printMe(ostream* out)	{	*out << "STR(" << lexeme << ")";	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a double
// constant.
//
// Version 1.0
// ----------------------------------------------------------
class DoubleConstingNode : public AbstractNode
{
public:
	DoubleConstingNode(string);
	void printMe(ostream*);

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};