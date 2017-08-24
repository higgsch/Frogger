// -----------------------------------------------------------------
// This is the header file for the BinaryNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\asciiNode.h"
using namespace std;

class FGRPhase;

// ----------------------------------------------------------
// This class provides representation for nodes with two 
// children.
//
// Version 3.1
// ----------------------------------------------------------
class BinaryNode : public AsciiNode
{
private:
	AsciiNode * leftChild; // this node's left child
	AsciiNode * rightChild; // this node's right child

protected:
	void addLeftChild(AsciiNode * absNode);
	void addRightChild(AsciiNode * absNode);

	void visitLeftChild(FGRPhase* p) { visitNode(p, leftChild); }
	void visitRightChild(FGRPhase* p) { visitNode(p, rightChild); }

public:
	BinaryNode(int lineNo);
	~BinaryNode();

	AsciiNode * getLeftChild() { return leftChild; }
	AsciiNode * getRightChild() { return rightChild; }

	bool isTreeTyped();

	void visitAllChildren(FGRPhase* p) { visitLeftChild(p); visitRightChild(p); }
};