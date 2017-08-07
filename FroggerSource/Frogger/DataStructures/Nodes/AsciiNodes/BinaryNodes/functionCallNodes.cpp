//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides nodes for function calls.
// -----------------------------------------------------------------
#include "functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This constructor builds a node for a function call.
// @str: The function's name.
//
// Version 5.0
// ----------------------------------------------------------
FunctionCallNode::FunctionCallNode(string str, int lineNo) : CommandCallNode(str, lineNo)
{
	funct = new Routine(DataType::DT_NOT_DEFINED, str, DataType::DT_NOT_DEFINED, false);
	((Routine*)getFunct())->copy(funct);
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