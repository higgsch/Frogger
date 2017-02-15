//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides nodes for function calls.
// -----------------------------------------------------------------
#include "functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 3.0
// ----------------------------------------------------------
Function::Function(DataType i_parentType, string i_name, DataType i_returnType) : Command(i_name)
{
	parentType = i_parentType;
	returnType = i_returnType;
}

// ----------------------------------------------------------
// This function determines if this function has the same
// signature as the given function.
// Note: return type is ignored because return type has to be
// unique.
// @funct: The function to compare.
//
// Version 3.0
// ----------------------------------------------------------
bool Function::equals(Function * funct)
{
	if (parentType != funct->parentType)
		return false;

	if (((Command *) this)->equals(funct))
		return true;

	return false;
}

// ----------------------------------------------------------
// This function determines if this function has a similar
// signature as the given function.
// Note: funct's DT_NOT_DEFINED data is ignored.
// @funct: The function to compare.
//
// Version 3.0
// ----------------------------------------------------------
bool Function::matches(Function * funct)
{
	if (funct->parentType != DT_NOT_DEFINED && parentType != funct->parentType)
		return false;
	if (funct->returnType != DT_NOT_DEFINED && returnType != funct->returnType)
		return false;

	if (((Command *)this)->matches(funct))
		return true;

	return false;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @funct: The function to duplicate.
//
// Version 3.0
// ----------------------------------------------------------
void Function::copy(Function * funct)
{
	((Command *)this)->copy(funct);
	parentType = funct->parentType;
	returnType = funct->returnType;
}

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