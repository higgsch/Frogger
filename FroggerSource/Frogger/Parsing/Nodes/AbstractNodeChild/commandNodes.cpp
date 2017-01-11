//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a display statement.
// @toDisplay: A node representing what should be displayed.
//
// Version 1.0
// ----------------------------------------------------------
DisplayingNode::DisplayingNode(AbstractNode* toDisplay)
{
	type = DISPLAYING;
	addLeftChild(toDisplay);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DisplayingNode::printMe(ostream* out)
{
	*out << "PRINT ";
	leftChild->printMe(out);
}