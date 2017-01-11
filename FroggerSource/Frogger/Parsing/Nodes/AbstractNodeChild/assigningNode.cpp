//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides a node representation for the 
// assignment operation.
// -----------------------------------------------------------------
#include "assigningNode.h"
#include "idRefNode.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for an assignment.
// @id: A node representing the variable to assign to.
// @toAssign: A node representing what to assign to the var.
//
// Version 1.0
// ----------------------------------------------------------
AssigningNode::AssigningNode(IdRefNode* id, AbstractNode* toAssign)
{
	type = ASSIGNING;
	addLeftChild(id);
	addRightChild(toAssign);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AssigningNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " = ";
	rightChild->printMe(out);
}