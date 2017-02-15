//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides nodes with one child.
// -----------------------------------------------------------------
#include "unaryNode.h"
using namespace std;

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
UnaryNode::~UnaryNode()
{
	if (child != NULL)
		delete child;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// child.
// @absNode: The new child for this node.
//
// Version 3.0
// ----------------------------------------------------------
void UnaryNode::addChild(AsciiNode * absNode)
{
	if (absNode == NULL)
		return;

	if (child == NULL)
		child = absNode;
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function determines if the tree down from this node
// has been successfully typed yet.
//
// Version 3.0
// ----------------------------------------------------------
bool UnaryNode::isTreeTyped()
{
	if (child != NULL && !child->isTreeTyped())
		return false;
	return isTyped();
}