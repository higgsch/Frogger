//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program provides the Symbol, Function, and Command classes.
// -----------------------------------------------------------------
#include "records.h"
using namespace std;

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 4.2
// ----------------------------------------------------------
Command::Command(string i_name)
{
	name = i_name;

	builtIn = false;
	argTypeList = new ArgList();
}

// ----------------------------------------------------------
// This function adds an argument type to the command's end.
// @argType: The data type of the argument to add.
//
// Version 4.2
// ----------------------------------------------------------
void Command::addArg(string argName, DataType argType)
{
	//if (argType == DT_NOT_DEFINED)
	//	return;
	
	argTypeList->push_back(new ArgPair(argName, argType));
}

// ----------------------------------------------------------
// This function determines if this command has the same
// signature as the given command.
// @cmd: The command to compare.
//
// Version 4.2
// ----------------------------------------------------------
bool Command::equals(Command * cmd)
{
	if (name != cmd->name)
		return false;

	if (argTypeList->size() != cmd->argTypeList->size())
		return false;

	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (argTypeList->at(i)->type != cmd->argTypeList->at(i)->type)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if this command has a similar
// signature as the given command.
// Note: cmd's DT_NOT_DEFINED data is ignored.
// @cmd: The command to compare.
//
// Version 4.2
// ----------------------------------------------------------
bool Command::matches(Command * cmd)
{
	if (name != cmd->name)
		return false;

	if (argTypeList->size() != cmd->argTypeList->size())
		return false;

	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (cmd->argTypeList->at(i)->type != DT_NOT_DEFINED && 
			argTypeList->at(i)->type != cmd->argTypeList->at(i)->type)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @cmd: The command to duplicate.
//
// Version 4.2
// ----------------------------------------------------------
void Command::copy(Command * cmd)
{
	builtIn = cmd->builtIn;
	name = cmd->name;

	argTypeList->clear();

	for (ArgPair* arg : *(cmd->argTypeList))
	{
		argTypeList->push_back(new ArgPair(arg->name, arg->type));
	}
}

// ----------------------------------------------------------
// This function returns the data type of the arg at the 
// selected index.
// @argNo: The index in the argument list to retrieve.
//
// Version 4.2
// ----------------------------------------------------------
DataType Command::getDataTypeOfArgNumber(int argNo)
{ 
	if (argNo < argTypeList->size())
		return argTypeList->at(argNo)->type;
	else
		return DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function sets the data type of the select arg index.
// @argNo: The index in the argument list to modify.
// @type: The data type to change to.
//
// Version 4.2
// ----------------------------------------------------------
void Command::setDataTypeOfArgNumber(int argNo, DataType type)
{ 
	if (argNo < argTypeList->size()) 
		(*argTypeList)[argNo]->type = type; 
}

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