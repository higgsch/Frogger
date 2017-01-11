// -----------------------------------------------------------------
// This is the header file for the IdRefNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a variable
// reference.
//
// Version 1.0
// ----------------------------------------------------------
class IdRefNode : public AbstractNode
{
public:
	IdRefNode(string);
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