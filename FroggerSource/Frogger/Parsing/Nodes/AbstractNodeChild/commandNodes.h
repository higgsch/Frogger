// -----------------------------------------------------------------
// This is the header file for the command node classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for getting
// double values from the user.
//
// Version 1.1
// ----------------------------------------------------------
class RetrievalNode : public AbstractNode
{
public:
	// ----------------------------------------------------------
	// This constructor builds a node for a retrieve statement.
	//
	// Version 1.1
	// ----------------------------------------------------------
	RetrievalNode()	{	type = RETRIEVING;	}

	// ----------------------------------------------------------
	// This function prints this node to the given output stream.
	// @out: The stream to display to.
	//
	// Version 1.1
	// ----------------------------------------------------------
	void printMe(ostream* out)	{	*out << "RETRIEVE";	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 1.1
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for the display
// statement.
//
// Version 1.0
// ----------------------------------------------------------
class DisplayingNode : public AbstractNode
{
public:
	DisplayingNode(AbstractNode*);
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

// ----------------------------------------------------------
// This class provides a node representation for getting
// a random number.
//
// Version 2.2
// ----------------------------------------------------------
class RandomingNode : public AbstractNode
{
public:
	// ----------------------------------------------------------
	// This constructor builds a node for a retrieve statement.
	//
	// Version 2.2
	// ----------------------------------------------------------
	RandomingNode()	{	type = RANDOMING;	}

	// ----------------------------------------------------------
	// This function prints this node to the given output stream.
	// @out: The stream to display to.
	//
	// Version 2.2
	// ----------------------------------------------------------
	void printMe(ostream* out)	{	*out << "RANDOM";	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.2
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for the end
// statement.
//
// Version 1.0
// ----------------------------------------------------------
class EndingNode : public AbstractNode
{
public:
	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 1.0
	// ----------------------------------------------------------
	EndingNode()	{	type = ENDING;	}

	// ----------------------------------------------------------
	// This function prints this node to the given output stream.
	// @out: The stream to display to.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void printMe(ostream* out)	{	*out << "END";	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};