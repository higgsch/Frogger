// -----------------------------------------------------------------
// This is the header for the FunctionTable class.
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "../Parsing/Nodes/AsciiNodes/asciiNode.h"
#include "../Parsing/Nodes/AsciiNodes/BinaryNodes/commandNodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a collection of commands
//
// Version 2.5
// ----------------------------------------------------------
class CommandTable
{
private:
	vector<Command*> * table;

public:
	CommandTable();
	void addCommand(Command * cmd);
	bool commandDefined(Command * cmd);
	bool matchExists(Command * cmd);
	int getNumberOfMatches(Command * cmd);
	Command* getFirstMatch(Command * cmd);
};