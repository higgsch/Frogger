//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 1.0
// -----------------------------------------------------------------
// This program provides a node for an identifier.
// -----------------------------------------------------------------
#include "idRefNode.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a variable reference.
// @id: The variable's textual representation.
//
// Version 1.0
// ----------------------------------------------------------
IdRefNode::IdRefNode(string id)
{
	type = IDREF;
	lexeme = id;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void IdRefNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	*out << "ID(" << lexeme << ")";

	if (parenNestCount > 0)
		*out << " )";
}