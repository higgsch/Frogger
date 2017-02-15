// -----------------------------------------------------------------
// This is the header file for the BinaryNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\asciiNode.h"
using namespace std;

class Phase;

// ----------------------------------------------------------
// This class provides representation for nodes with two 
// children.
//
// Version 3.0
// ----------------------------------------------------------
class BinaryNode : public AsciiNode
{
private:
	AsciiNode * leftChild; // this node's left child
	AsciiNode * rightChild; // this node's right child

protected:
	void addLeftChild(AsciiNode * absNode);
	void addRightChild(AsciiNode * absNode);

	void visitLeftChild(Phase* p) { visitNode(p, leftChild); }
	void visitRightChild(Phase* p) { visitNode(p, rightChild); }

public:
	BinaryNode();
	~BinaryNode();

	AsciiNode * getLeftChild() { return leftChild; }
	AsciiNode * getRightChild() { return rightChild; }

	bool isTreeTyped();

	void visitAllChildren(Phase* p) { visitLeftChild(p); visitRightChild(p); }
};