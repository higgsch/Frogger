// -----------------------------------------------------------------
// This is the header for Node Record classes.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "..\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class represents everything known about a variable.
//
// Version 3.0
// ----------------------------------------------------------
struct Symbol
{
	string id;
	DataType type;
	bool isLocal; //flag for a symbol defined within a user-defined function

	Symbol(string i_id, DataType i_type) { id = i_id; type = i_type;}
	bool equals(Symbol* s) { return id == s->id; }
	bool matches(Symbol* s) { return id == s->id; }
	//TODO fix matches
};

// ----------------------------------------------------------
// This class represents everything known about a command.
//
// Version 4.0
// ----------------------------------------------------------
class Command
{
public:
	bool builtIn;
	string name;
	vector<DataType> *argTypeList;

	Command(string i_name);
	void addArg(DataType arg);
	bool equals(Command* cmd);
	bool matches(Command* cmd);
	void copy(Command* cmd);

	int getNumArgs();
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