//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.1
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a command call.
// @str: The command's name.
//
// Version 5.1
// ----------------------------------------------------------
CommandCallNode::CommandCallNode(DataType * primaryType, string str, int lineNo) : BinaryNode(lineNo)
{
	lexeme = str;
	cmd = new Routine(primaryType, str, DataType::DT_NULL, false);
	isParentScope = false;
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
CommandCallNode::~CommandCallNode()
{
	if (cmd != NULL)
		delete cmd;
}