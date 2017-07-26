//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program provides the Symbol, Function, and Command classes.
// -----------------------------------------------------------------
#include "records.h"
using namespace std;

// ----------------------------------------------------------
// This function adds an argument type to the routine's end.
// @argName: The name of the argument to add.
// @argType: The data type of the argument to add.
//
// Version 4.2
// ----------------------------------------------------------
void Routine::addArg(string argName, DataType argType)
{
	//if (argType == DT_NOT_DEFINED)
	//	return;
	
	argTypeList->push_back(new ArgPair(argName, argType));
}

// ----------------------------------------------------------
// This function determines if this Routine has the same
// signature as the given routine.
// @other: The routine to compare.
//
// Version 4.2
// ----------------------------------------------------------
bool Routine::equals(Routine * other)
{
	if (parentType != other->parentType)
		return false;

	if (name != other->name)
		return false;

	if (argTypeList->size() != other->argTypeList->size())
		return false;

	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (argTypeList->at(i)->type != other->argTypeList->at(i)->type)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if this routine has a similar
// signature as the given routine.
// Note: other's DT_NOT_DEFINED data is ignored.
// @other: The routine to compare.
//
// Version 4.2
// ----------------------------------------------------------
bool Routine::matches(Routine * other)
{
	if (other->parentType != DT_NOT_DEFINED && parentType != other->parentType)
		return false;

	if (name != other->name)
		return false;

	if (other->returnType != DT_NOT_DEFINED && returnType != other->returnType)
		return false;

	if (argTypeList->size() != other->argTypeList->size())
		return false;

	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (other->argTypeList->at(i)->type != DT_NOT_DEFINED && 
			argTypeList->at(i)->type != other->argTypeList->at(i)->type)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @other: The Routine to duplicate.
//
// Version 4.2
// ----------------------------------------------------------
void Routine::copy(Routine * other)
{
	builtIn = other->builtIn;
	parentType = other->parentType;
	name = other->name;
	returnType = other->returnType;

	argTypeList->clear();

	for (ArgPair* arg : *(other->argTypeList))
	{
		argTypeList->push_back(new ArgPair(arg->name, arg->type));
	}
}

// ----------------------------------------------------------
// This function returns the arg at the selected index.
// @argNo: The index in the argument list to retrieve.
//
// Version 4.2
// ----------------------------------------------------------
ArgPair* Routine::getArg(int argNo)
{ 
	if (argNo < argTypeList->size())
		return argTypeList->at(argNo);
	else
		return NULL;
}

// ----------------------------------------------------------
// This function sets the data type of the select arg index.
// @argNo: The index in the argument list to modify.
// @type: The data type to change to.
//
// Version 4.2
// ----------------------------------------------------------
void Routine::setDataTypeOfArgNumber(int argNo, DataType type)
{ 
	if (argNo < argTypeList->size()) 
		(*argTypeList)[argNo]->type = type; 
}