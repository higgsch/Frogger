//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides nodes for arithmetic operations.
// -----------------------------------------------------------------
#include "arithmeticNodes.h"
using namespace std;

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AddingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " + ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void SubingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " - ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void MulingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " * ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DivingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " / ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.1
// ----------------------------------------------------------
void ModDivingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " % ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.1
// ----------------------------------------------------------
void IDivingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " \\ ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.1
// ----------------------------------------------------------
void RootingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " ROOT ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.1
// ----------------------------------------------------------
void ExpingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " ^ ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}