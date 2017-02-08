//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program provides a node representation for the 
// assignment operation.
// -----------------------------------------------------------------
#include "assigningNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a double assignment.
// @id: A node representing the variable to assign to.
// @toAssign: A node representing what to assign to the var.
//
// Version 2.3
// ----------------------------------------------------------
AssigningDoubleNode::AssigningDoubleNode(IdRefNode* id, AbstractNode* toAssign)
{
	type = ASSIGNINGDOUBLE;
	addLeftChild(id);
	addRightChild(toAssign);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AssigningDoubleNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " = ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This constructor builds a node for a string assignment.
// @id: A node representing the variable to assign to.
// @toAssign: A node representing what to assign to the var.
//
// Version 2.3
// ----------------------------------------------------------
AssigningStringNode::AssigningStringNode(IdRefNode* id, AbstractNode* toAssign)
{
	type = ASSIGNINGSTRING;
	addLeftChild(id);
	addRightChild(toAssign);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AssigningStringNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " = ";
	rightChild->printMe(out);
}