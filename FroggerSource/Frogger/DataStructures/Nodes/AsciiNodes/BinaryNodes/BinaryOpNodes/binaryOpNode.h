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

typedef enum binaryOps {
	ADD, SUB, MUL, DIV, MODDIV, IDIV, ROOT,
	EXP, NOT, LT, GT, EQ, LTE, GTE
} binaryOp;

// ----------------------------------------------------------
// This class provides a node representation for a binary
// operation; such as addition and division.
//
// Version 4.2
// ----------------------------------------------------------
class BinaryOpNode : public BinaryNode
{
protected:
	binaryOp op;
public:
	BinaryOpNode(int lineNo, binaryOp op) : BinaryNode(lineNo), op(op) {}
	~BinaryOpNode() {}

	void addOps(AsciiNode* left, AsciiNode* right) {
		addLeftChild(left); addRightChild(right); }

	AsciiNode* getLeftOperand() { return getLeftChild(); }
	AsciiNode* getRightOperand() { return getRightChild(); }

	binaryOp getOpType() { return op; }

	void visitLeftOperand(Phase* p) { visitLeftChild(p); }
	void visitRightOperand(Phase* p) { visitRightChild(p); }
	void visitBothOperands(Phase* p) { visitAllChildren(p); }
};