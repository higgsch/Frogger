//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides nodes for constants.
// -----------------------------------------------------------------
#include "constingNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a string constant.
// @toDisplay: The string's textual value.
//
// Version 1.0
// ----------------------------------------------------------
StringConstingNode::StringConstingNode(string str)
{
	type = STRINGCONSTING;
	lexeme = str;
}

// ----------------------------------------------------------
// This constructor builds a node for a double constant.
// @dbl: The textual representation of the double.
//
// Version 1.0
// ----------------------------------------------------------
DoubleConstingNode::DoubleConstingNode(string dbl)
{
	type = DBLCONSTING;
	lexeme = dbl;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DoubleConstingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	*out << "DBL(" << lexeme << ")";

	if (parenNestCount > 0)
		*out << " )";
}