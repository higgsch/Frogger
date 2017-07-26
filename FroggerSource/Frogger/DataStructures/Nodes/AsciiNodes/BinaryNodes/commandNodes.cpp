//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a command call.
// @str: The command's name.
//
// Version 4.2
// ----------------------------------------------------------
CommandCallNode::CommandCallNode(string str, int lineNo) : BinaryNode(lineNo)
{
	lexeme = str;
	cmd = new Command(DT_NULL, str, false);
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