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
// Version 5.0
// ----------------------------------------------------------
struct SymbolRecord : public Record<Symbol>
{
	SymbolRecord(Symbol* s) : Record(s) {}
	SymbolRecord(string id, DataType * type, bool isLocal) 
		: Record(new Symbol(id, type, isLocal)) {}

	bool equals(Symbol* s) {return rec->equals(s);}
	bool matches(Symbol* s) {return rec->matches(s);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Command for Table use.
//
// Version 5.0
// ----------------------------------------------------------
struct CommandRecord : public Record<Routine>
{
	CommandRecord(Routine* c) : Record(c) {}

	bool equals(Routine* c) {return rec->equals(c);}
	bool matches(Routine* c) {return rec->matches(c);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Function for Table use.
//
// Version 5.0
// ----------------------------------------------------------
struct FunctionRecord : public Record<Routine>
{
	FunctionRecord(Routine* f) : Record(f) {}

	bool equals(Routine* f) {return rec->equals(f);}
	bool matches(Routine* f) {return rec->matches(f);}

	bool isAddable();
};

class Language;
struct UDFRecord;

// ----------------------------------------------------------
// This class represents a collection of variables
//
// Version 5.0
// ----------------------------------------------------------
class SymbolTable : public Table<Symbol>
{
public:
	SymbolTable(Language * lang, UDFRecord * rec);
	SymbolTable(Language * lang);
	SymbolTable() {}

	DataType * symbolType(string id);

	Symbol* SYM_ARGS;
};

// ----------------------------------------------------------
// This class represents a collection of commands
//
// Version 5.0
// ----------------------------------------------------------
class CommandTable : public Table<Routine>
{
private:
	void initializeConsts(Language * lang);

public:
	CommandTable(Language * lang);
	CommandTable() {}

	void add(Routine* r) { ((Table<Routine>*)this)->add(new CommandRecord(r)); }
	void addBuiltInVisibleCommands();

	void addEndNull() { add(CMD_END_NULL); }
	void addEndString() { add(CMD_END_STR); }
	void addEndDouble() { add(CMD_END_DBL); }

	Routine* CMD_END_NULL;
	Routine* CMD_END_STR;
	Routine* CMD_END_DBL;
	Routine* CMD_DISPLAY_STR;
	Routine* CMD_DISPLAY_DBL;
	Routine* CMD_OPEN_INPUT;
	Routine* CMD_CLOSE_INPUT;
	Routine* CMD_WRITE;
	Routine* CMD_OPEN_OUTPUT;
	Routine* CMD_CLOSE_OUTPUT;
};

// ----------------------------------------------------------
// This class represents a collection of functions
//
// Version 5.0
// ----------------------------------------------------------
class FunctionTable : public Table<Routine>
{
private:
	void initializeConsts(Language * lang);

public:
	FunctionTable(Language * lang);
	FunctionTable() {}

	void add(Routine* r) { ((Table<Routine>*)this)->add(new FunctionRecord(r)); }
	void addBuiltInVisibleFunctions();

	DataType* getFunctionReturnType(Routine* funct);

	Routine* FUNCT_TO_STRING;
	Routine* FUNCT_TO_ASCII;
	Routine* FUNCT_PARSE_DOUBLE;
	Routine* FUNCT_ASCII_AT;
	Routine* FUNCT_LENGTH;
	Routine* FUNCT_RETRIEVE_DOUBLE;
	Routine* FUNCT_RANDOM;
	Routine* FUNCT_RETRIEVE_STRING;
	Routine* FUNCT_READ;
	Routine* FUNCT_ELEMENT_AT;
	Routine* FUNCT_SIZE;
};