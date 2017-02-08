// -----------------------------------------------------------------
// This is the header file for the AssigningNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
#include "idRefNode.h"
using namespace std;

class IdRefNode;

// ----------------------------------------------------------
// This class provides a node representation for the double
// assignment operation.
//
// Version 2.3
// ----------------------------------------------------------
class AssigningDoubleNode : public AbstractNode
{
public:
	AssigningDoubleNode(IdRefNode*, AbstractNode*);
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
// This class provides a node representation for the string
// assignment operation.
//
// Version 2.3
// ----------------------------------------------------------
class AssigningStringNode : public AbstractNode
{
public:
	AssigningStringNode(IdRefNode*, AbstractNode*);
	void printMe(ostream*);

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.3
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
};