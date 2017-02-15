//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides nodes with two children.
// -----------------------------------------------------------------
#include "binaryNode.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
BinaryNode::BinaryNode()
{
	leftChild = NULL;
	rightChild = NULL;
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
BinaryNode::~BinaryNode()
{
	if (leftChild != NULL)
		delete leftChild;

	if (rightChild != NULL)
		delete rightChild;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// left child.
// @absNode: The new left child for this node.
//
// Version 3.0
// ----------------------------------------------------------
void BinaryNode::addLeftChild(AsciiNode * absNode)
{
	if (absNode == NULL)
		return;

	if (leftChild == NULL)
		leftChild = absNode;
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// right child.
// @absNode: The new right child for this node.
//
// Version 3.0
// ----------------------------------------------------------
void BinaryNode::addRightChild(AsciiNode * absNode)
{
	if (absNode == NULL)
		return;

	if (rightChild == NULL)
		rightChild = absNode;
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function determines if the tree down from this node
// has been successfully typed yet.
//
// Version 3.0
// ----------------------------------------------------------
bool BinaryNode::isTreeTyped()
{
	if (leftChild != NULL && !leftChild->isTreeTyped())
		return false;
	if (rightChild != NULL && !rightChild->isTreeTyped())
		return false;
	return isTyped();
}