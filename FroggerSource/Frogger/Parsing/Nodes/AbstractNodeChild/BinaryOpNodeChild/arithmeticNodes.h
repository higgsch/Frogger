// -----------------------------------------------------------------
// This is the header file for the arithmetic nodes. 
// -----------------------------------------------------------------
#pragma once

#include "..\binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for an addition
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class AddingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AddingNode()	{	type = ADDING;	}
		
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
// This class provides a node representation for a subtraction
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class SubingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 1.0
	// ----------------------------------------------------------
	SubingNode()	{	type = SUBING;	}

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
// This class provides a node representation for a multiplication
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class MulingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 1.0
	// ----------------------------------------------------------
	MulingNode()	{	type = MULING;	}

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
// This class provides a node representation for a division
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class DivingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 1.0
	// ----------------------------------------------------------
	DivingNode()	{	type = DIVING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};
