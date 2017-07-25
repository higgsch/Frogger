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
// Version 2.5
// ----------------------------------------------------------
Command::Command(string i_name)
{
	name = i_name;

	builtIn = false;
	argTypeList = NULL;
}

// ----------------------------------------------------------
// This function adds an argument type to the command's end.
// @argType: The data type of the argument to add.
//
// Version 4.2
// ----------------------------------------------------------
void Command::addArg(DataType argType)
{
	//if (argType == DT_NOT_DEFINED)
	//	return;

	if (argTypeList == NULL)
		argTypeList = new DataTypeList();

	argTypeList->push_back(argType);
}

// ----------------------------------------------------------
// This function determines if this command has the same
// signature as the given command.
// @cmd: The command to compare.
//
// Version 2.5
// ----------------------------------------------------------
bool Command::equals(Command * cmd)
{
	if (name != cmd->name)
		return false;
	if (argTypeList == NULL && cmd->argTypeList != NULL)
		return false;
	if (argTypeList != NULL && cmd->argTypeList == NULL)
		return false;
	if (argTypeList != NULL && cmd->argTypeList != NULL)
	{
		if (argTypeList->size() != cmd->argTypeList->size())
			return false;
		for (int i = 0; i < argTypeList->size(); i++)
		{
			if (argTypeList->at(i) != cmd->argTypeList->at(i))
				return false;
		}
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if this command has a similar
// signature as the given command.
// Note: cmd's DT_NOT_DEFINED data is ignored.
// @cmd: The command to compare.
//
// Version 4.0
// ----------------------------------------------------------
bool Command::matches(Command * cmd)
{
	if (name != cmd->name)
		return false;
	if (cmd->argTypeList == NULL)
		return argTypeList == NULL;
	if (argTypeList == NULL)
		return false;

	if (argTypeList->size() != cmd->argTypeList->size())
		return false;

	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (cmd->argTypeList->at(i) != DT_NOT_DEFINED && argTypeList->at(i) != cmd->argTypeList->at(i))
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
	if (cmd->argTypeList != NULL)
	{
		if (argTypeList == NULL)
			argTypeList = new DataTypeList();
		else
			argTypeList->clear();

		for (DataType type : *(cmd->argTypeList))
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
// This function returns the number of arguments to the command.
//
// Version 2.5
// ----------------------------------------------------------
int Command::getNumArgs()
{ 
	if (argTypeList != NULL)
		return argTypeList->size(); 
	else
		return 0;
}

// ----------------------------------------------------------
// This function returns the data type of the arg at the 
// selected index.
// @argNo: The index in the argument list to retrieve.
//
// Version 2.5
// ----------------------------------------------------------
DataType Command::getDataTypeOfArgNumber(int argNo)
{ 
	if (argTypeList == NULL)
		return DT_NOT_DEFINED;
	else
		return argTypeList->at(argNo);
}

// ----------------------------------------------------------
// This function sets the data type of the select arg index.
// @argNo: The index in the argument list to modify.
// @type: The data type to change to.
//
// Version 2.5
// ----------------------------------------------------------
void Command::setDataTypeOfArgNumber(int argNo, DataType type)
{ 
	if (argNo < argTypeList->size()) 
		(*argTypeList)[argNo] = type; 
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