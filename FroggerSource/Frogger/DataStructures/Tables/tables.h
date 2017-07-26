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
class Language;

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 4.2
// ----------------------------------------------------------
struct UDFRecord
{
	string UDFName;
	ArgList * args;
	DataType returnType;

	UDFRecord() { args = new ArgList(); }

	ArgPair* operator[](int index) { return (*args)[index]; }
};

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function Collection.
//
// Version 4.2
// ----------------------------------------------------------
struct UDFCollection : vector<UDFRecord *>
{

};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Program. Generated from the .struct file.
//
// Version 4.2
// ----------------------------------------------------------
struct ProgramStruct
{
	UDFRecord * PEF;
	UDFCollection * UDFs;

	ProgramStruct() { UDFs = new UDFCollection(); }

	int getNumberOfUDFs() { return UDFs->size(); }
	UDFRecord * getUDF(int udfIndex) { return (*UDFs)[udfIndex]; }

	UDFRecord* operator[](int index) { return (*UDFs)[index]; }
};

// ----------------------------------------------------------
// This class wraps a Symbol for Table use.
//
// Version 4.2
// ----------------------------------------------------------
struct SymbolRecord : public Record<Symbol>
{
	SymbolRecord(Symbol* s) : Record(s) {}
	SymbolRecord(string id, DataType type, bool isLocal) 
		: Record(new Symbol(id, type, isLocal)) {}

	bool equals(Symbol* s) {return rec->equals(s);}
	bool matches(Symbol* s) {return rec->matches(s);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Command for Table use.
//
// Version 4.2
// ----------------------------------------------------------
struct CommandRecord : public Record<Command>
{
	CommandRecord(Command* c) : Record(c) {}
	CommandRecord(DataType parentType, string name, bool builtIn)
		: Record(new Command(parentType, name, builtIn)) {}

	bool equals(Command* c) {return rec->equals(c);}
	bool matches(Command* c) {return rec->matches(c);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class wraps a Function for Table use.
//
// Version 4.2
// ----------------------------------------------------------
struct FunctionRecord : public Record<Function>
{
	FunctionRecord(Function* f) : Record(f) {}
	FunctionRecord(DataType parentType, string name, DataType returnType, bool builtIn)
		: Record(new Function(parentType, name, returnType, builtIn)) {}

	bool equals(Function* f) {return rec->equals(f);}
	bool matches(Function* f) {return rec->matches(f);}

	bool isAddable();
};

// ----------------------------------------------------------
// This class represents a collection of variables
//
// Version 4.2
// ----------------------------------------------------------
class SymbolTable : public Table<Symbol>
{
private:
	Language * lang;
public:
	SymbolTable(Language * language, UDFRecord * rec);
	SymbolTable(Language * language);

	DataType symbolType(string id);

	Symbol* SYM_ARGS;
};

// ----------------------------------------------------------
// This class represents a collection of commands
//
// Version 4.2
// ----------------------------------------------------------
class CommandTable : public Table<Command>
{
private:
	Language * lang;
public:
	CommandTable(Language * language);

	Command* CMD_END_NULL;
	Command* CMD_END_STR;
	Command* CMD_END_DBL;
	Command* CMD_DISPLAY_STR;
	Command* CMD_DISPLAY_DBL;
	Command* CMD_OPEN_INPUT;
	Command* CMD_CLOSE_INPUT;
	Command* CMD_WRITE;
	Command* CMD_OPEN_OUTPUT;
	Command* CMD_CLOSE_OUTPUT;
};

// ----------------------------------------------------------
// This class represents a collection of functions
//
// Version 4.2
// ----------------------------------------------------------
class FunctionTable : public Table<Function>
{
private:
	Language * lang;
public:
	FunctionTable(Language * language);

	DataType getFunctionReturnType(Function* funct);

	Function* FUNCT_TO_STRING;
	Function* FUNCT_TO_ASCII;
	Function* FUNCT_PARSE_DOUBLE;
	Function* FUNCT_ASCII_AT;
	Function* FUNCT_LENGTH;
	Function* FUNCT_RETRIEVE_DOUBLE;
	Function* FUNCT_RANDOM;
	Function* FUNCT_RETRIEVE_STRING;
	Function* FUNCT_READ;
	Function* FUNCT_ELEMENT_AT;
	Function* FUNCT_SIZE;
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
// This class wraps the ASTs and Symbol Tables for a UDF Collection.
//
// Version 4.2
// ----------------------------------------------------------
struct FunctionASTCollection : vector<FunctionAST *>
{

};

// ----------------------------------------------------------
// This class wraps the PEF and all UDFs in a Frogger program.
//
// Version 4.2
// ----------------------------------------------------------
struct ProgramAST
{
	FunctionAST * PEF;
	FunctionASTCollection * UDFs;

	ProgramAST() { UDFs = new FunctionASTCollection(); }

	int getNumberOfUDFs() { return UDFs->size(); }

	ProgramNode* getUDFNode(int udfIndex) { return (*UDFs)[udfIndex]->root; }
	ProgramNode* getPEFNode() { return PEF->root; }

	FunctionAST* operator[](int index) { return (*UDFs)[index]; }
};