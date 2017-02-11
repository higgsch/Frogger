//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.5
// -----------------------------------------------------------------
// This program provides nodes for FROGGER commands.
// -----------------------------------------------------------------
#include "commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This is a straight-through constructor.
//
// Version 2.5
// ----------------------------------------------------------
Command::Command(string i_name)
{
	builtIn = false;
	name = i_name;
	argTypeList = NULL;
}

// ----------------------------------------------------------
// This function adds an argument type to the command's end.
// @argType: The data type of the argument to add.
//
// Version 2.5
// ----------------------------------------------------------
void Command::addArg(DataType argType)
{
	//if (argType == DT_NOT_DEFINED)
	//	return;

	if (argTypeList == NULL)
		argTypeList = new vector<DataType>();

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
	if (argTypeList->size() != cmd->argTypeList->size())
		return false;
	for (int i = 0; i < argTypeList->size(); i++)
	{
		if (argTypeList->at(i) != cmd->argTypeList->at(i))
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
// Version 2.5
// ----------------------------------------------------------
bool Command::matches(Command * cmd)
{
	if (name != cmd->name)
		return false;
	if (cmd->argTypeList != NULL && argTypeList->size() != cmd->argTypeList->size())
		return false;

	if (cmd->argTypeList != NULL)
	{
		for (int i = 0; i < argTypeList->size(); i++)
		{
			if (cmd->argTypeList->at(i) != DT_NOT_DEFINED && argTypeList->at(i) != cmd->argTypeList->at(i))
				return false;
		}
	}

	return true;
}

// ----------------------------------------------------------
// This function performs a deep copy to itself.
// @cmd: The command to duplicate.
//
// Version 2.5
// ----------------------------------------------------------
void Command::copy(Command * cmd)
{
	builtIn = cmd->builtIn;
	name = cmd->name;
	if (cmd->argTypeList != NULL)
	{
		if (argTypeList == NULL)
			argTypeList = new vector<DataType>();
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
// This constructor builds a node for a command call.
// @str: The command's name.
//
// Version 2.5
// ----------------------------------------------------------
CommandCallNode::CommandCallNode(string str)
{
	type = COMMANDCALLING;
	lexeme = str;
	cmd = new Command(str);
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
// Version 2.5
// ----------------------------------------------------------
void ArgListNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << ", ";
	rightChild->printMe(out);
}