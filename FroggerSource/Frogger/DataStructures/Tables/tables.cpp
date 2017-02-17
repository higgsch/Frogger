//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program provides the SymbolTable, FunctionTable, and 
// CommandTable.
// -----------------------------------------------------------------
#include "tables.h"
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
// Version 3.0
// ----------------------------------------------------------
bool CommandRecord::isAddable()
{
	if (rec->argTypeList != NULL)
	{
		for (DataType t : *(rec->argTypeList))
		{
			if (t == DT_NOT_DEFINED)
				return false;
		}
	}

	return true;
}

// ----------------------------------------------------------
// This function determines if the current record is allowed
// to be added to the table.
//
// Version 3.0
// ----------------------------------------------------------
bool FunctionRecord::isAddable()
{
	if (rec->parentType == DT_NOT_DEFINED)
		return false;

	if (rec->returnType == DT_NOT_DEFINED)
		return false;

	if (rec->argTypeList != NULL)
	{
		for (DataType t : *(rec->argTypeList))
		{
			if (t == DT_NOT_DEFINED)
				return false;
		}
	}

	return true;
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
// Version 3.0
// ----------------------------------------------------------
CommandTable::CommandTable()
{
	//Add built-in commands
	CMD_END->builtIn = true;
	add(new CommandRecord(CMD_END));

	CMD_DISPLAY_STR->argTypeList = new vector<DataType>();
	CMD_DISPLAY_STR->addArg(DT_STRING);
	CMD_DISPLAY_STR->builtIn = true;
	add(new CommandRecord(CMD_DISPLAY_STR));

	CMD_DISPLAY_DBL->argTypeList = new vector<DataType>();
	CMD_DISPLAY_DBL->addArg(DT_DOUBLE);
	CMD_DISPLAY_DBL->builtIn = true;
	add(new CommandRecord(CMD_DISPLAY_DBL));
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 3.0
// ----------------------------------------------------------
FunctionTable::FunctionTable()
{
	//Add built-in functions
	FUNCT_TO_STRING->builtIn = true;
	add(new FunctionRecord(FUNCT_TO_STRING));

	FUNCT_TO_ASCII->builtIn = true;
	add(new FunctionRecord(FUNCT_TO_ASCII));

	FUNCT_PARSE_DOUBLE->builtIn = true;
	add(new FunctionRecord(FUNCT_PARSE_DOUBLE));

	FUNCT_ASCII_AT->argTypeList = new vector<DataType>();
	FUNCT_ASCII_AT->addArg(DT_DOUBLE);
	FUNCT_ASCII_AT->builtIn = true;
	add(new FunctionRecord(FUNCT_ASCII_AT));

	FUNCT_RETRIEVE_DOUBLE->builtIn = true;
	add(new FunctionRecord(FUNCT_RETRIEVE_DOUBLE));

	FUNCT_RANDOM->builtIn = true;
	add(new FunctionRecord(FUNCT_RANDOM));

	FUNCT_RETRIEVE_STRING->builtIn = true;
	add(new FunctionRecord(FUNCT_RETRIEVE_STRING));
}

// ----------------------------------------------------------
// This function returns the return type of the desired 
// function or DataType::DT_NOT_DEFINED if the function is not 
// in the table.
// @funct: The function in question.
//
// Version 3.0
// ----------------------------------------------------------
DataType FunctionTable::getFunctionReturnType(Function* funct)
{
	for (Record<Function>* f : *table)
	{
		if (f->equals(funct))
			return funct->returnType;
	}

	return DT_NOT_DEFINED;
}

Command* CommandTable::CMD_END			= new Command("end");
Command* CommandTable::CMD_DISPLAY_STR	= new Command("display");
Command* CommandTable::CMD_DISPLAY_DBL	= new Command("display");

Function* FunctionTable::FUNCT_TO_STRING		= new Function(DT_DOUBLE, "toString", DT_STRING);
Function* FunctionTable::FUNCT_TO_ASCII			= new Function(DT_DOUBLE, "toAscii", DT_STRING);
Function* FunctionTable::FUNCT_PARSE_DOUBLE		= new Function(DT_STRING, "parseDouble", DT_DOUBLE);
Function* FunctionTable::FUNCT_ASCII_AT			= new Function(DT_STRING, "asciiAt", DT_DOUBLE);
Function* FunctionTable::FUNCT_RETRIEVE_DOUBLE	= new Function(DT_NULL, "retrieveDouble", DT_DOUBLE);
Function* FunctionTable::FUNCT_RANDOM			= new Function(DT_NULL, "random", DT_DOUBLE);
Function* FunctionTable::FUNCT_RETRIEVE_STRING	= new Function(DT_NULL, "retrieveString", DT_STRING);