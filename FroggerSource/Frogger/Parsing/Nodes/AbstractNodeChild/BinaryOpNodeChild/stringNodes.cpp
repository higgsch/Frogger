//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program provides nodes for arithmetic operations.
// -----------------------------------------------------------------
#include "stringNodes.h"
using namespace std;

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.3
// ----------------------------------------------------------
void StringConcatingNode::printMe(ostream* out)
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
// Version 2.3
// ----------------------------------------------------------
void DoubleConcatingNode::printMe(ostream* out)
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
// Version 2.3
// ----------------------------------------------------------
void AsciiConcatingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " + ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}