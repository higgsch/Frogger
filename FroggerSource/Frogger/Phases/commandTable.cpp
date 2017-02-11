//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.5
// ----------------------------------------------------------------
// This program represents a collection of commands
// -----------------------------------------------------------------
#include "commandTable.h"
using namespace std;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 2.5
// ----------------------------------------------------------
CommandTable::CommandTable()
{
	table = new vector<Command*>();

	//Add built-in commands
	Command * end = new Command("end");
	end->builtIn = true;
	table->push_back(end);

	Command * displayString = new Command("display");
	displayString->argTypeList = new vector<DataType>();
	displayString->addArg(DT_STRING);
	displayString->builtIn = true;
	table->push_back(displayString);

	Command * displayDouble = new Command("display");
	displayDouble->argTypeList = new vector<DataType>();
	displayDouble->addArg(DT_DOUBLE);
	displayDouble->builtIn = true;
	table->push_back(displayDouble);
}

// ----------------------------------------------------------
// This function adds a command to the table.
// @cmd: The command to add.
//
// Version 2.5
// ----------------------------------------------------------
void CommandTable::addCommand(Command * cmd)
{
	//if (cmd->parentType == DT_NOT_DEFINED)
	//	return;

	//Skip non-well-defined commands
	for (DataType t : *(cmd->argTypeList))
	{
		if (t == DT_NOT_DEFINED)
			return;
	}

	if (!commandDefined(cmd))
		table->push_back(cmd);
}

// ----------------------------------------------------------
// This function determines if the command already exists in 
// the table.
// @cmd: The command in question.
//
// Version 2.5
// ----------------------------------------------------------
bool CommandTable::commandDefined(Command * cmd)
{
	for (Command* c : *table)
	{
		if (c->equals(cmd))
			return true;
	}
	return false;
}

// ----------------------------------------------------------
// This function determines if the command matches a defined
// command
// @cmd: The command in question.
//
// Version 2.5
// ----------------------------------------------------------
bool CommandTable::matchExists(Command * cmd)
{
	for (Command* c : *table)
	{
		if (c->matches(cmd))
			return true;
	}
	return false;
}

// ----------------------------------------------------------
// This function returns the number of defined commands that
// match the desired command.
// @cmd: The command in question.
//
// Version 2.5
// ----------------------------------------------------------
int CommandTable::getNumberOfMatches(Command * cmd)
{
	int matches = 0;
	for (Command* c : *table)
	{
		if (c->matches(cmd))
			matches++;
	}
	return matches;
}

// ----------------------------------------------------------
// This function returns the first matching command in the 
// table or NULL if no match found.
// @cmd: The command in question.
//
// Version 2.5
// ----------------------------------------------------------
Command * CommandTable::getFirstMatch(Command * cmd)
{
	for (Command* c : *table)
	{
		if (c->matches(cmd))
			return c;
	}
	return NULL;
}