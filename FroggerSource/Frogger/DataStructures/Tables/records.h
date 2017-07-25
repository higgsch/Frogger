// -----------------------------------------------------------------
// This is the header for Node Record classes.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "..\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class represents the data known about an argument.
//
// Version 4.2
// ----------------------------------------------------------
struct ArgPair
{
	string name;
	DataType type;

	ArgPair(string argName, DataType argType) : name(argName), type(argType) {}
};

// ----------------------------------------------------------
// This class represents the data known about an argument list.
//
// Version 4.2
// ----------------------------------------------------------
struct ArgList : vector<ArgPair *>
{
	bool isEmpty() { return size() == 0; }
};

// ----------------------------------------------------------
// This class represents everything known about a variable.
//
// Version 4.2
// ----------------------------------------------------------
struct Symbol
{
	string id;
	DataType type;
	bool isLocal; //flag for a symbol defined within a user-defined function

	Symbol(string i_id, DataType i_type) : id(i_id), type(i_type), isLocal(true) {}
	bool equals(Symbol* s) { return id == s->id; }
	bool matches(Symbol* s) { return id == s->id; }
	//TODO fix matches
};

// ----------------------------------------------------------
// This class represents everything known about a command.
//
// Version 4.2
// ----------------------------------------------------------
class Command
{
public:
	bool builtIn;
	string name;
	ArgList *argTypeList;

	Command(string i_name);
	void addArg(string argName, DataType argType);
	bool equals(Command* cmd);
	bool matches(Command* cmd);
	void copy(Command* cmd);

	int getNumArgs() { return argTypeList->size(); }
	DataType getDataTypeOfArgNumber(int argNo);
	void setDataTypeOfArgNumber(int argNo, DataType type);

	bool isUserDefined() { return !builtIn; }
};

// ----------------------------------------------------------
// This class represents everything known about a Function.
//
// Version 3.0
// ----------------------------------------------------------
class Function : public Command
{
public:
	DataType parentType;
	DataType returnType;

	Function(DataType i_parentType, string i_name, DataType i_returnType);
	bool equals(Function* funct);
	bool matches(Function* funct);
	void copy(Function* funct);
};