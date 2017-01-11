//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides nodes for logical operations.
// -----------------------------------------------------------------
#include "logicNodes.h"
using namespace std;

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void NotingNode::printMe(ostream* out)
{
	*out << " ! [";
	leftChild->printMe(out);
	*out << "] ";
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void LTingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " < ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void GTingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " > ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void EQingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " == ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void LTEingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " <= ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void GTEingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " >= ";
	rightChild->printMe(out);
}