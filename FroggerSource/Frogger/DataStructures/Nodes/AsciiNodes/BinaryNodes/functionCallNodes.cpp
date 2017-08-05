//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.1
// -----------------------------------------------------------------
// This program provides nodes for function calls.
// -----------------------------------------------------------------
#include "functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a function call.
// @str: The function's name.
//
// Version 4.2
// ----------------------------------------------------------
FunctionCallNode::FunctionCallNode(string str, int lineNo) : CommandCallNode(str, lineNo)
{
	funct = new Function(DT_NOT_DEFINED, str, DT_NOT_DEFINED, false);
	((Command*)getFunct())->copy(funct);
}

// ----------------------------------------------------------
// Destructor.
//
// Version 3.0
// ----------------------------------------------------------
FunctionCallNode::~FunctionCallNode()
{
	if (funct != NULL)
		delete funct;
}