//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.4
// -----------------------------------------------------------------
// This program provides nodes for function calls.
// -----------------------------------------------------------------
#include "functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 2.4
// ----------------------------------------------------------
Function::Function(DataType i_parentType, string i_name, DataType i_returnType)
{
	builtIn = false;
	parentType = i_parentType;
	name = i_name;
	returnType = i_returnType;
	argTypeList = NULL;
}

// ----------------------------------------------------------
// This function adds an argument type to the function's end.
// @argType: The data type of the argument to add.
//
// Version 2.4
// ----------------------------------------------------------
void Function::addArg(DataType argType)
{
	if (argType == DT_NOT_DEFINED)
		return;

	argTypeList->push_back(argType);
}

// ----------------------------------------------------------
// This function determines if this function has the same
// signature as the given function.
// Note: return type is ignored because return type has to be
// unique.
// @funct: The function to compare.
//
// Version 2.4
// ----------------------------------------------------------
bool Function::equals(Function * funct)
{
	if (parentType != funct->parentType)
		return false;
	if (name != funct->name)
		return false;
	if (argTypeList->size() != funct->argTypeList->size())
		return false;
	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (argTypeList->at(i) != funct->argTypeList->at(i))
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if this function has a similar
// signature as the given function.
// Note: funct's DT_NOT_DEFINED data is ignored.
// @funct: The function to compare.
//
// Version 2.4
// ----------------------------------------------------------
bool Function::matches(Function * funct)
{
	if (funct->parentType != DT_NOT_DEFINED && parentType != funct->parentType)
		return false;
	if (name != funct->name)
		return false;
	if (funct->returnType != DT_NOT_DEFINED && returnType != funct->returnType)
		return false;
	if (funct->argTypeList != NULL && argTypeList->size() != funct->argTypeList->size())
		return false;

	if (funct->argTypeList != NULL)
	{
		for (int i = 0; i < argTypeList->size(); i++)
		{
			if (funct->argTypeList->at(i) != DT_NOT_DEFINED && argTypeList->at(i) != funct->argTypeList->at(i))
				return false;
		}
	}

	return true;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @funct: The function to duplicate.
//
// Version 2.4
// ----------------------------------------------------------
void Function::copy(Function * funct)
{
	builtIn = funct->builtIn;
	parentType = funct->parentType;
	name = funct->name;
	returnType = funct->returnType;
	if (funct->argTypeList != NULL)
	{
		if (argTypeList == NULL)
			argTypeList = new vector<DataType>();
		else
			argTypeList->clear();

		for (DataType type : *(funct->argTypeList))
		{
			argTypeList->push_back(type);
		}
	}
	else
	{
		if (argTypeList != NULL)
		{
			delete argTypeList;
			argTypeList = NULL;
		}
	}
}

// ----------------------------------------------------------
// This constructor builds a node for a function call.
// @toDisplay: The function's name.
//
// Version 2.4
// ----------------------------------------------------------
FunctionCallNode::FunctionCallNode(string str)
{
	type = FUNCTCALLING;
	lexeme = str;
	funct = new Function(DT_NOT_DEFINED, str, DT_NOT_DEFINED);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.4
// ----------------------------------------------------------
void FunctionCallNode::printMe(ostream* out)
{
	leftChild->printMe(out);

	if (rightChild != NULL)
	{
		*out << ", ";
		rightChild->printMe(out);
	}
}

// ----------------------------------------------------------
// This constructor builds a node for an element in an
// argument list.
// @dbl: The textual representation of the double.
//
// Version 2.4
// ----------------------------------------------------------
ArgListNode::ArgListNode()
{
	type = ARGLISTING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.4
// ----------------------------------------------------------
void ArgListNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << "." << lexeme << "(";
	rightChild->printMe(out);
	*out << ")";
}