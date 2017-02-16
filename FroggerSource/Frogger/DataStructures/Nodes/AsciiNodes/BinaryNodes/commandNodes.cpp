//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a command call.
// @str: The command's name.
//
// Version 3.0
// ----------------------------------------------------------
CommandCallNode::CommandCallNode(string str)
{
	lexeme = str;
	cmd = new Command(str);
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