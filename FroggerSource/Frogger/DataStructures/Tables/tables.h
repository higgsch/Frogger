// -----------------------------------------------------------------
// This is the header for table classes.
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "table.h"
#include "records.h"
#include "..\dataTyped.h"
using namespace std;

//forward declaration
class ProgramNode;

// ----------------------------------------------------------
// This class represents the data known about an argument.
//
// Version 4.0
// ----------------------------------------------------------
struct argPair
{
	string name;
	DataType type;
};

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 4.0
// ----------------------------------------------------------
struct UDFRecord
{
	string UDFName;
	vector<argPair *> * args;
	DataType returnType;
};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Program. Generated from the .struct file.
//
// Version 4.0
// ----------------------------------------------------------
struct ProgramStruct
{
	UDFRecord * PEF;
	vector<UDFRecord *> * UDFs;
};

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
// Version 4.0
// ----------------------------------------------------------
class SymbolTable : public Table<Symbol>
{
public:
	SymbolTable(UDFRecord * rec);

	DataType symbolType(string id);
};

// ----------------------------------------------------------
// This class represents a collection of commands
//
// Version 3.2
// ----------------------------------------------------------
class CommandTable : public Table<Command>
{
public:
	CommandTable();

	static Command* CMD_END;
	static Command* CMD_DISPLAY_STR;
	static Command* CMD_DISPLAY_DBL;
	static Command* CMD_OPEN_INPUT;
	static Command* CMD_CLOSE_INPUT;
	static Command* CMD_WRITE;
	static Command* CMD_OPEN_OUTPUT;
	static Command* CMD_CLOSE_OUTPUT;

};

// ----------------------------------------------------------
// This class represents a collection of functions
//
// Version 3.3
// ----------------------------------------------------------
class FunctionTable : public Table<Function>
{
public:
	FunctionTable();

	DataType getFunctionReturnType(Function* funct);

	static Function* FUNCT_TO_STRING;
	static Function* FUNCT_TO_ASCII;
	static Function* FUNCT_PARSE_DOUBLE;
	static Function* FUNCT_ASCII_AT;
	static Function* FUNCT_LENGTH;
	static Function* FUNCT_RETRIEVE_DOUBLE;
	static Function* FUNCT_RANDOM;
	static Function* FUNCT_RETRIEVE_STRING;
	static Function* FUNCT_READ;
	static Function* FUNCT_ELEMENT_AT;
	static Function* FUNCT_SIZE;
};

// ----------------------------------------------------------
// This class wraps the AST and Symbol Table for a UDF.
//
// Version 4.0
// ----------------------------------------------------------
struct FunctionAST
{
	ProgramNode * root;
	SymbolTable * symbols;
};

// ----------------------------------------------------------
// This class wraps the PEF and all UDFs in a Frogger program.
//
// Version 4.0
// ----------------------------------------------------------
struct ProgramAST
{
	FunctionAST * PEF;
	vector<FunctionAST *> * UDFs;
};