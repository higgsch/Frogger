// -----------------------------------------------------------------
// This is the header file for the AssigningNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
#include "idRefNode.h"
using namespace std;

class IdRefNode;

// ----------------------------------------------------------
// This class provides a node representation for the 
// assignment operation.
//
// Version 1.0
// ----------------------------------------------------------
class AssigningNode : public AbstractNode
{
public:
	AssigningNode(IdRefNode*, AbstractNode*);
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