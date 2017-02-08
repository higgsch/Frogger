// -----------------------------------------------------------------
// This is the header file for the arithmetic nodes. 
// -----------------------------------------------------------------
#pragma once

#include "..\binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for string 
// concatenation.
//
// Version 2.3
// ----------------------------------------------------------
class StringConcatingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.3
	// ----------------------------------------------------------
	StringConcatingNode()	{	type = STRINGCONCATING;	}
		
	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.3
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for double 
// concatenation.
//
// Version 2.3
// ----------------------------------------------------------
class DoubleConcatingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.3
	// ----------------------------------------------------------
	DoubleConcatingNode()	{	type = DOUBLECONCATING;	}
		
	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.3
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for ascii 
// concatenation.
//
// Version 2.3
// ----------------------------------------------------------
class AsciiConcatingNode : public BinaryOpNode
{
public:
	void printMe(ostream*);

	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.3
	// ----------------------------------------------------------
	AsciiConcatingNode()	{	type = ASCIICONCATING;	}
		
	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.3
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};