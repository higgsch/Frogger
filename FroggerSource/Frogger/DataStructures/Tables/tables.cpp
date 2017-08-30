//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the SymbolTable, FunctionTable, and 
// CommandTable.
// -----------------------------------------------------------------
#include "tables.h"
#include "structs.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 5.0
// ----------------------------------------------------------
bool SymbolRecord::isAddable()
{ 
	return rec->type != DataType::DT_NOT_DEFINED; 
}

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 5.0
// ----------------------------------------------------------
bool CommandRecord::isAddable()
{
	for (ArgPair* a : *(rec->args))
	{
		if (a->type == DataType::DT_NOT_DEFINED)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 5.0
// ----------------------------------------------------------
bool FunctionRecord::isAddable()
{
	if (rec->primary == DataType::DT_NOT_DEFINED)
		return false;

	if (rec->returnType == DataType::DT_NOT_DEFINED)
		return false;

	for (ArgPair* a : *(rec->args))
	{
		if (a->type == DataType::DT_NOT_DEFINED)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function creates a symbol table prepopulated with
// the argument list in the given UDFRecord.
// @rec: The UDFRecord to copy symbols from.
//
// Version 5.0
// ----------------------------------------------------------
SymbolTable::SymbolTable(Language * lang, UDFRecord * rec)
{
	SYM_ARGS = new Symbol(lang->SYM_ARGS, DataType::DT_STRINGLIST, false);
	add(new SymbolRecord(SYM_ARGS));

	//Add all UDF arguments
	int numArgs = rec->args->size();
	for (int index = 0; index < numArgs; index++)
	{
		ArgPair * pair = (*rec)[index];
		add(new SymbolRecord(pair->name, pair->type, false));
	}
}

// ----------------------------------------------------------
// This function creates a symbol table prepopulated with
// args.
// @language: The Language the symbol table is for.
//
// Version 5.0
// ----------------------------------------------------------
SymbolTable::SymbolTable(Language * lang)
{
	SYM_ARGS = new Symbol(lang->SYM_ARGS, DataType::DT_STRINGLIST, false);
	add(new SymbolRecord(SYM_ARGS));
}

// ----------------------------------------------------------
// This function returns the data type of the desired symbol
// or DataType::DT_NOT_DEFINED if the symbol is not in the 
// table.
// @id: The string representation of the variable in question.
//
// Version 5.0
// ----------------------------------------------------------
DataType * SymbolTable::symbolType(string id)
{
	Symbol s = Symbol(id, DataType::DT_NOT_DEFINED, false);

	Symbol* found = getFirstMatch(&s);
	if (found != NULL)
		return found->type;

	return DataType::DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 5.0
// ----------------------------------------------------------
CommandTable::CommandTable(Language * lang)
{
	initializeConsts(lang);
	addBuiltInVisibleCommands();
}

// ----------------------------------------------------------
// This function adds the built in commands for visible tables
//
// Version 5.0
// ----------------------------------------------------------
void CommandTable::addBuiltInVisibleCommands()
{
	add(CMD_DISPLAY_STR);
	add(CMD_DISPLAY_DBL);
	add(CMD_OPEN_INPUT);
	add(CMD_CLOSE_INPUT);
	add(CMD_WRITE);
	add(CMD_OPEN_OUTPUT);
	add(CMD_CLOSE_OUTPUT);
}

// ----------------------------------------------------------
// This function creates and initializes the CMD constants.
// @lang: The language the CommandTable is in.
//
// Version 5.0
// ----------------------------------------------------------
void CommandTable::initializeConsts(Language * lang)
{
	CMD_END_NULL = new Routine(DataType::DT_NULL, lang->CMDNAME_END_NULL, DataType::DT_NULL, true);

	CMD_END_STR = new Routine(DataType::DT_NULL, lang->CMDNAME_END_STR, DataType::DT_NULL, true);
	CMD_END_STR->addArg("", DataType::DT_STRING);

	CMD_END_DBL = new Routine(DataType::DT_NULL, lang->CMDNAME_END_DBL, DataType::DT_NULL, true);
	CMD_END_DBL->addArg("", DataType::DT_DOUBLE);
	
	CMD_DISPLAY_STR = new Routine(DataType::DT_NULL, lang->CMDNAME_DISPLAY_STR, DataType::DT_NULL, true);
	CMD_DISPLAY_STR->addArg("", DataType::DT_STRING);

	CMD_DISPLAY_DBL = new Routine(DataType::DT_NULL, lang->CMDNAME_DISPLAY_DBL, DataType::DT_NULL, true);
	CMD_DISPLAY_DBL->addArg("", DataType::DT_DOUBLE);

	CMD_OPEN_INPUT = new Routine(DataType::DT_NULL, lang->CMDNAME_OPEN_INPUT, DataType::DT_NULL, true);
	CMD_OPEN_INPUT->addArg("", DataType::DT_STRING);

	CMD_CLOSE_INPUT = new Routine(DataType::DT_NULL, lang->CMDNAME_CLOSE_INPUT, DataType::DT_NULL, true);
	
	CMD_WRITE = new Routine(DataType::DT_NULL, lang->CMDNAME_WRITE, DataType::DT_NULL, true);
	CMD_WRITE->addArg("", DataType::DT_STRING);

	CMD_OPEN_OUTPUT = new Routine(DataType::DT_NULL, lang->CMDNAME_OPEN_OUTPUT, DataType::DT_NULL, true);
	CMD_OPEN_OUTPUT->addArg("", DataType::DT_STRING);

	CMD_CLOSE_OUTPUT = new Routine(DataType::DT_NULL, lang->CMDNAME_CLOSE_OUTPUT, DataType::DT_NULL, true);
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 5.0
// ----------------------------------------------------------
FunctionTable::FunctionTable(Language * lang)
{
	initializeConsts(lang);

	addBuiltInVisibleFunctions();
}

// ----------------------------------------------------------
// This function adds the built in functions for visible tables
//
// Version 5.0
// ----------------------------------------------------------
void FunctionTable::addBuiltInVisibleFunctions()
{
	add(FUNCT_RETRIEVE_DOUBLE);
	add(FUNCT_RANDOM);
	add(FUNCT_RETRIEVE_STRING);
	add(FUNCT_READ);
}

// ----------------------------------------------------------
// This function creates and initializes the FUNCT constants.
// @lang: The language the FunctionTable is in.
//
// Version 5.0
// ----------------------------------------------------------
void FunctionTable::initializeConsts(Language* lang)
{
	FUNCT_TO_STRING = new Routine(DataType::DT_DOUBLE, lang->FUNCTNAME_TO_STRING, DataType::DT_STRING, true);

	FUNCT_TO_ASCII = new Routine(DataType::DT_DOUBLE, lang->FUNCTNAME_TO_ASCII, DataType::DT_STRING, true);

	FUNCT_PARSE_DOUBLE = new Routine(DataType::DT_STRING, lang->FUNCTNAME_PARSE_DOUBLE, DataType::DT_DOUBLE, true);

	FUNCT_ASCII_AT = new Routine(DataType::DT_STRING, lang->FUNCTNAME_ASCII_AT, DataType::DT_DOUBLE, true);
	FUNCT_ASCII_AT->addArg("", DataType::DT_DOUBLE);

	FUNCT_LENGTH = new Routine(DataType::DT_STRING, lang->FUNCTNAME_LENGTH, DataType::DT_DOUBLE, true);

	FUNCT_RETRIEVE_DOUBLE = new Routine(DataType::DT_NULL, lang->FUNCTNAME_RETRIEVE_DOUBLE, DataType::DT_DOUBLE, true);

	FUNCT_RANDOM = new Routine(DataType::DT_NULL, lang->FUNCTNAME_RANDOM, DataType::DT_DOUBLE, true);

	FUNCT_RETRIEVE_STRING = new Routine(DataType::DT_NULL, lang->FUNCTNAME_RETRIEVE_STRING, DataType::DT_STRING, true);

	FUNCT_READ = new Routine(DataType::DT_NULL, lang->FUNCTNAME_READ, DataType::DT_STRING, true);

	FUNCT_ELEMENT_AT = new Routine(DataType::DT_STRINGLIST, lang->FUNCTNAME_ELEMENT_AT, DataType::DT_STRING, true);
	FUNCT_ELEMENT_AT->addArg("", DataType::DT_DOUBLE);
	
	FUNCT_SIZE = new Routine(DataType::DT_STRINGLIST, lang->FUNCTNAME_SIZE, DataType::DT_DOUBLE, true);
}

// ----------------------------------------------------------
// This function returns the return type of the desired 
// function or DataType::DT_NOT_DEFINED if the function is not 
// in the table.
// @funct: The function in question.
//
// Version 5.0
// ----------------------------------------------------------
DataType* FunctionTable::getFunctionReturnType(Routine* funct)
{
	for (Record<Routine>* f : *this)
	{
		if (f->equals(funct))
			return funct->returnType;
	}

	return DataType::DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function merges all three tables of the given group 
// into the calling group.
// @other: The group of tables to merge into this.
//
// Version 5.0
// ----------------------------------------------------------
void TableGroup::merge(TableGroup * other)
{
	cmds->merge(other->cmds);
	functs->merge(other->functs);
	syms->merge(other->syms);
}