//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a command call.
// @str: The command's name.
//
// Version 5.0
// ----------------------------------------------------------
CommandCallNode::CommandCallNode(string str, int lineNo) : BinaryNode(lineNo)
{
	lexeme = str;
	cmd = new Command(DataType::DT_NULL, str, false);
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