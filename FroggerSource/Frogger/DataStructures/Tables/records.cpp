//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
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
// Version 5.0
// ----------------------------------------------------------
void Routine::addArg(string argName, DataType * argType)
{
	//if (argType == DT_NOT_DEFINED)
	//	return;
	
	args->push_back(new ArgPair(argName, argType));
}

// ----------------------------------------------------------
// This function determines if this Routine has the same
// signature as the given routine.
// @other: The routine to compare.
//
// Version 5.0
// ----------------------------------------------------------
bool Routine::equals(Routine * other)
{
	if (primary != other->primary)
		return false;

	if (name != other->name)
		return false;

	if (args->size() != other->args->size())
		return false;

	for (int i = 0; i < args->size(); i++)
	{
		if (args->at(i)->type != other->args->at(i)->type)
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
// Version 5.0
// ----------------------------------------------------------
bool Routine::matches(Routine * other)
{
	if (other->primary != DataType::DT_NOT_DEFINED && *primary != *(other->primary))
		return false;

	if (name != other->name)
		return false;

	if (other->returnType != DataType::DT_NOT_DEFINED && *returnType != *(other->returnType))
		return false;

	if (args->size() != other->args->size())
		return false;

	for (int i = 0; i < args->size(); i++)
	{
		if (other->args->at(i)->type != DataType::DT_NOT_DEFINED && 
			*(args->at(i)->type) != *(other->args->at(i)->type))
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @other: The Routine to duplicate.
//
// Version 5.0
// ----------------------------------------------------------
void Routine::copy(Routine * other)
{
	builtIn = other->builtIn;
	primary = other->primary;
	name = other->name;
	returnType = other->returnType;

	args->clear();

	for (ArgPair* arg : *(other->args))
	{
		args->push_back(new ArgPair(arg->name, arg->type));
	}
}

// ----------------------------------------------------------
// This function returns the arg at the selected index.
// @argNo: The index in the argument list to retrieve.
//
// Version 5.0
// ----------------------------------------------------------
ArgPair* Routine::getArg(int argNo)
{ 
	if (argNo < args->size())
		return args->at(argNo);
	else
		return NULL;
}

// ----------------------------------------------------------
// This function sets the data type of the select arg index.
// @argNo: The index in the argument list to modify.
// @type: The data type to change to.
//
// Version 5.0
// ----------------------------------------------------------
void Routine::setDataTypeOfArgNumber(int argNo, DataType* type)
{ 
	if (argNo < args->size()) 
		(*args)[argNo]->type = type; 
}