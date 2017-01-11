//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides a node representation for a binary
// operation; such as addition and division.
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a binary
// operation; such as addition and division.
//
// Version 1.0
// ----------------------------------------------------------
class BinaryOpNode : public AbstractNode
{
public:
	// ----------------------------------------------------------
	// This function adds the left and right operands to the AST.
	// @left: The left side of the operator.
	// @right: The right side of the operator.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void addOps(AbstractNode* left, AbstractNode* right)
	{
		addLeftChild(left);
		addRightChild(right);
	}
};