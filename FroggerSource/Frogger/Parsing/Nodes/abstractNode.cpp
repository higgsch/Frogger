//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program provides the base of the node inheritance for
// the AST intermediate representation. Supports the visitor
// pattern for a Phase.
// -----------------------------------------------------------------

#include "abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor initializes pointers and counters.
//
// Version 2.3
// ----------------------------------------------------------
AbstractNode::AbstractNode()
{
	parent = NULL;
	leftChild = NULL;
	rightChild = NULL;
	parenNestCount = 0;
	dataType = DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// left child.
// @absNode: The new left child for this node.
//
// Version 2.0
// ----------------------------------------------------------
void AbstractNode::addLeftChild(AbstractNode * absNode)
{
	if (absNode == NULL)
		return;
	if (leftChild == NULL)
	{
		absNode->setParent(this);
		absNode->setWhichSide(LEFT);
		leftChild = absNode;
	}
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// right child.
// @absNode: The new right child for this node.
//
// Version 2.0
// ----------------------------------------------------------
void AbstractNode::addRightChild(AbstractNode * absNode)
{
	if (absNode == NULL)
		return;
	if (rightChild == NULL)
	{
		absNode->setParent(this);
		absNode->setWhichSide(RIGHT);
		rightChild = absNode;

	}
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 1.0
// ----------------------------------------------------------
void AbstractNode::clean()
{
	if (leftChild != NULL)
	{
		leftChild->clean();
		free(leftChild);
	}
	if (rightChild != NULL)
	{
		rightChild->clean();
		free(rightChild);
	}
}