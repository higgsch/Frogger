//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.1
// -----------------------------------------------------------------
// This program provides a node representation for a binary
// operation; such as addition and division.
// -----------------------------------------------------------------
#pragma once

#include "..\binaryNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a binary
// operation; such as addition and division.
//
// Version 3.1
// ----------------------------------------------------------
class BinaryOpNode : public BinaryNode
{
public:
	BinaryOpNode(int lineNo) : BinaryNode(lineNo) {}
	~BinaryOpNode() {}

	void addOps(AsciiNode* left, AsciiNode* right) {
		addLeftChild(left); addRightChild(right); }

	AsciiNode* getLeftOperand() { return getLeftChild(); }
	AsciiNode* getRightOperand() { return getRightChild(); }

	void visitLeftOperand(Phase* p) { visitLeftChild(p); }
	void visitRightOperand(Phase* p) { visitRightChild(p); }
	void visitBothOperands(Phase* p) { visitAllChildren(p); }
};