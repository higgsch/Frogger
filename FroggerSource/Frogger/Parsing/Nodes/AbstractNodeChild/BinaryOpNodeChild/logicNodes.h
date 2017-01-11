// -----------------------------------------------------------------
// This is the header file for the boolean logic classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a boolean not
// operation.
//
// Version 2.0
// ----------------------------------------------------------
class NotingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	NotingNode()	{	type = NOTING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a less than
// comparison.
//
// Version 2.0
// ----------------------------------------------------------
class LTingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	LTingNode()	{	type = LTING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than comparison.
//
// Version 2.0
// ----------------------------------------------------------
class GTingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	GTingNode()	{	type = GTING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for an equivalence
// comparison.
//
// Version 2.0
// ----------------------------------------------------------
class EQingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	EQingNode()	{	type = EQING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a less than
// or equal comparison.
//
// Version 2.0
// ----------------------------------------------------------
class LTEingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	LTEingNode()	{	type = LTEING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than or equal comparison.
//
// Version 2.0
// ----------------------------------------------------------
class GTEingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	GTEingNode()	{	type = GTEING;	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};