// -----------------------------------------------------------------
// This is the header for table classes.
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "table.h"
#include "records.h"
#include "..\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class wraps a Symbol for Table use.
//
// Version 3.0
// ----------------------------------------------------------
struct SymbolRecord : public Record<Symbol>
{
	SymbolRecord(Symbol* s) : Record(s) {}
	bool equals(Symbol* s) {return rec->equals(s);}
	bool matches(Symbol* s) {return rec->matches(s);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Command for Table use.
//
// Version 3.0
// ----------------------------------------------------------
struct CommandRecord : public Record<Command>
{
	CommandRecord(Command* c) : Record(c) {}
	bool equals(Command* c) {return rec->equals(c);}
	bool matches(Command* c) {return rec->matches(c);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Function for Table use.
//
// Version 3.0
// ----------------------------------------------------------
struct FunctionRecord : public Record<Function>
{
	FunctionRecord(Function* f) : Record(f) {}
	bool equals(Function* f) {return rec->equals(f);}
	bool matches(Function* f) {return rec->matches(f);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class represents a collection of variables
//
// Version 3.0
// ----------------------------------------------------------
class SymbolTable : public Table<Symbol>
{
public:
	SymbolTable() {}

	DataType symbolType(string id);
};

// ----------------------------------------------------------
// This class represents a collection of commands
//
// Version 3.0
// ----------------------------------------------------------
class CommandTable : public Table<Command>
{
public:
	CommandTable();
};

// ----------------------------------------------------------
// This class represents a collection of functions
//
// Version 3.0
// ----------------------------------------------------------
class FunctionTable : public Table<Function>
{
public:
	FunctionTable();

	DataType getFunctionReturnType(Function* funct);
};