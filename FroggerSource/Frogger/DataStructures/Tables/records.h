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
// Version 4.2
// ----------------------------------------------------------
struct Symbol
{
	string id;
	DataType type;
	bool isLocal; //flag for a symbol defined within a user-defined function

	Symbol(string i_id, DataType i_type, bool i_isLocal) 
		: id(i_id), type(i_type), isLocal(i_isLocal) {}

	bool equals(Symbol* s) { return id == s->id; }
	bool matches(Symbol* s) { return id == s->id; }
	//TODO fix matches
};

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
// This class represents everything known about a Routine.
//
// Version 4.2
// ----------------------------------------------------------
struct Routine
{
	bool builtIn;
	DataType parentType;
	string name;
	DataType returnType;
	ArgList * argTypeList;

	Routine(DataType i_parentType, string i_name, DataType i_returnType, bool i_builtIn)
		: parentType(i_parentType), name(i_name), returnType(i_returnType), builtIn(i_builtIn),
		  argTypeList(new ArgList()) {}

	void addArg(string argName, DataType argType);
	
	bool equals(Routine* other);
	bool matches(Routine* other);
	void copy(Routine* other);

	int getNumArgs() { return argTypeList->size(); }
	ArgPair * getArg(int argNo);
	void setDataTypeOfArgNumber(int argNo, DataType type);

	bool isUserDefined() { return !builtIn; }
};

struct Function : public Routine 
{
	Function(DataType i_parentType, string i_name, DataType i_returnType, bool i_builtIn)
		: Routine(i_parentType, i_name, i_returnType, i_builtIn) {}
};

struct Command : public Routine 
{
	Command(DataType i_parentType, string i_name, bool i_builtIn)
		: Routine(i_parentType, i_name, DT_NULL, i_builtIn) {}
};