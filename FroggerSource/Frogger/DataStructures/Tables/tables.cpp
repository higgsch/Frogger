//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program provides the SymbolTable, FunctionTable, and 
// CommandTable.
// -----------------------------------------------------------------
#include "tables.h"
#include "..\OutputLanguage\language.h"
using namespace std;

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 3.0
// ----------------------------------------------------------
bool SymbolRecord::isAddable()
{ 
	return rec->type != DT_NOT_DEFINED; 
}

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 4.2
// ----------------------------------------------------------
bool CommandRecord::isAddable()
{
	for (ArgPair* a : *(rec->argTypeList))
	{
		if (a->type == DT_NOT_DEFINED)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 4.2
// ----------------------------------------------------------
bool FunctionRecord::isAddable()
{
	if (rec->parentType == DT_NOT_DEFINED)
		return false;

	if (rec->returnType == DT_NOT_DEFINED)
		return false;

	for (ArgPair* a : *(rec->argTypeList))
	{
		if (a->type == DT_NOT_DEFINED)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function creates a symbol table prepopulated with
// the argument list in the given UDFRecord.
// @rec: The UDFRecord to copy symbols from.
//
// Version 4.2
// ----------------------------------------------------------
SymbolTable::SymbolTable(Language * language, UDFRecord * rec)
{
	lang = language;
	Symbol* argsSym = new Symbol(lang->SYM_ARGS, DT_ARGS);
	argsSym->isLocal = false;
	add(new SymbolRecord(argsSym));
	int index = 0;

	while (index < rec->args->size())
	{
		ArgPair * pair = (*rec)[index];
		Symbol * sym = new Symbol(pair->name, pair->type);
		sym->isLocal = false;
		add(new SymbolRecord(sym));
		index++;
	}
}

// ----------------------------------------------------------
// This function creates a symbol table prepopulated with
// args.
// @language: The Language the symbol table is for.
//
// Version 4.2
// ----------------------------------------------------------
SymbolTable::SymbolTable(Language * language)
{
	lang = language;
	Symbol* argsSym = new Symbol(lang->SYM_ARGS, DT_ARGS);
	argsSym->isLocal = false;
	add(new SymbolRecord(argsSym));
}

// ----------------------------------------------------------
// This function returns the data type of the desired symbol
// or DataType::DT_NOT_DEFINED if the symbol is not in the 
// table.
// @id: The string representation of the variable in question.
//
// Version 3.0
// ----------------------------------------------------------
DataType SymbolTable::symbolType(string id)
{
	Symbol s = Symbol(id, DT_NOT_DEFINED);

	Symbol* found = getFirstMatch(&s);
	if (found != NULL)
		return found->type;

	return DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 4.2
// ----------------------------------------------------------
CommandTable::CommandTable(Language * language)
{
	lang = language;

	CMD_END_NULL = new Command(DT_NULL, lang->CMDNAME_END_NULL, true);
	CMD_END_STR = new Command(DT_NULL, lang->CMDNAME_END_STR, true);
	CMD_END_DBL = new Command(DT_NULL, lang->CMDNAME_END_DBL, true);
	CMD_DISPLAY_STR = new Command(DT_NULL, lang->CMDNAME_DISPLAY_STR, true);
	CMD_DISPLAY_DBL = new Command(DT_NULL, lang->CMDNAME_DISPLAY_DBL, true);
	CMD_OPEN_INPUT = new Command(DT_NULL, lang->CMDNAME_OPEN_INPUT, true);
	CMD_CLOSE_INPUT = new Command(DT_NULL, lang->CMDNAME_CLOSE_INPUT, true);
	CMD_WRITE = new Command(DT_NULL, lang->CMDNAME_WRITE, true);
	CMD_OPEN_OUTPUT = new Command(DT_NULL, lang->CMDNAME_OPEN_OUTPUT, true);
	CMD_CLOSE_OUTPUT = new Command(DT_NULL, lang->CMDNAME_CLOSE_OUTPUT, true);

	//Add built-in commands
	add(new CommandRecord(CMD_END_NULL));
	
	CMD_END_STR->addArg("", DT_STRING);
	add(new CommandRecord(CMD_END_STR));
	
	CMD_END_DBL->addArg("", DT_DOUBLE);
	add(new CommandRecord(CMD_END_DBL));

	CMD_DISPLAY_STR->addArg("", DT_STRING);
	add(new CommandRecord(CMD_DISPLAY_STR));

	CMD_DISPLAY_DBL->addArg("", DT_DOUBLE);
	add(new CommandRecord(CMD_DISPLAY_DBL));

	CMD_OPEN_INPUT->addArg("", DT_STRING);
	add(new CommandRecord(CMD_OPEN_INPUT));

	add(new CommandRecord(CMD_CLOSE_INPUT));

	CMD_WRITE->addArg("", DT_STRING);
	add(new CommandRecord(CMD_WRITE));

	CMD_OPEN_OUTPUT->addArg("", DT_STRING);
	add(new CommandRecord(CMD_OPEN_OUTPUT));

	add(new CommandRecord(CMD_CLOSE_OUTPUT));
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 4.2
// ----------------------------------------------------------
FunctionTable::FunctionTable(Language * language)
{
	lang = language;

	FUNCT_TO_STRING = new Function(DT_DOUBLE, lang->FUNCTNAME_TO_STRING, DT_STRING, true);
	FUNCT_TO_ASCII = new Function(DT_DOUBLE, lang->FUNCTNAME_TO_ASCII, DT_STRING, true);
	FUNCT_PARSE_DOUBLE = new Function(DT_STRING, lang->FUNCTNAME_PARSE_DOUBLE, DT_DOUBLE, true);
	FUNCT_ASCII_AT = new Function(DT_STRING, lang->FUNCTNAME_ASCII_AT, DT_DOUBLE, true);
	FUNCT_LENGTH = new Function(DT_STRING, lang->FUNCTNAME_LENGTH, DT_DOUBLE, true);
	FUNCT_RETRIEVE_DOUBLE = new Function(DT_NULL, lang->FUNCTNAME_RETRIEVE_DOUBLE, DT_DOUBLE, true);
	FUNCT_RANDOM = new Function(DT_NULL, lang->FUNCTNAME_RANDOM, DT_DOUBLE, true);
	FUNCT_RETRIEVE_STRING = new Function(DT_NULL, lang->FUNCTNAME_RETRIEVE_STRING, DT_STRING, true);
	FUNCT_READ = new Function(DT_NULL, lang->FUNCTNAME_READ, DT_STRING, true);
	FUNCT_ELEMENT_AT = new Function(DT_ARGS, lang->FUNCTNAME_ELEMENT_AT, DT_STRING, true);
	FUNCT_SIZE = new Function(DT_ARGS, lang->FUNCTNAME_SIZE, DT_DOUBLE, true);

	//Add built-in functions
	add(new FunctionRecord(FUNCT_TO_STRING));

	add(new FunctionRecord(FUNCT_TO_ASCII));

	add(new FunctionRecord(FUNCT_PARSE_DOUBLE));

	FUNCT_ASCII_AT->addArg("", DT_DOUBLE);
	add(new FunctionRecord(FUNCT_ASCII_AT));

	add(new FunctionRecord(FUNCT_LENGTH));

	add(new FunctionRecord(FUNCT_RETRIEVE_DOUBLE));

	add(new FunctionRecord(FUNCT_RANDOM));

	add(new FunctionRecord(FUNCT_RETRIEVE_STRING));

	add(new FunctionRecord(FUNCT_READ));

	FUNCT_ELEMENT_AT->addArg("", DT_DOUBLE);
	add(new FunctionRecord(FUNCT_ELEMENT_AT));

	add(new FunctionRecord(FUNCT_SIZE));
}

// ----------------------------------------------------------
// This function returns the return type of the desired 
// function or DataType::DT_NOT_DEFINED if the function is not 
// in the table.
// @funct: The function in question.
//
// Version 4.2
// ----------------------------------------------------------
DataType FunctionTable::getFunctionReturnType(Function* funct)
{
	for (Record<Function>* f : *this)
	{
		if (f->equals(funct))
			return funct->returnType;
	}

	return DT_NOT_DEFINED;
}