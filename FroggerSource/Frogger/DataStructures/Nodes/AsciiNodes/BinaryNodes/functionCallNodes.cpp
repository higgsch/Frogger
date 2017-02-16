//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides nodes for function calls.
// -----------------------------------------------------------------
#include "functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a function call.
// @str: The function's name.
//
// Version 3.0
// ----------------------------------------------------------
FunctionCallNode::FunctionCallNode(string str) : CommandCallNode(str)
{
	funct = new Function(DT_NOT_DEFINED, str, DT_NOT_DEFINED);
	((Command*)this->getFunct())->copy(funct);
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