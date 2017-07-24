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
// This function creates a symbol table prepopulated with
// the argument list in the given UDFRecord.
// @rec: The UDFRecord to copy symbols from.
//
// Version 4.2
// ----------------------------------------------------------
SymbolTable::SymbolTable(Language * language, UDFRecord * rec)
{
	lang = language;
	Symbol* argsSym = new Symbol(lang->SYM_ARGS.getText(),DT_ARGS);
	argsSym->isLocal = false;
	add(new SymbolRecord(argsSym));
	int index = 0;

	while (index < rec->args->size())
	{
		argPair * pair = (*(rec->args))[index];
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
	Symbol* argsSym = new Symbol(lang->SYM_ARGS.getText(),DT_ARGS);
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

	CMD_END_NULL		= new Command(lang->CMDNAME_END_NULL.getText());
	CMD_END_STR		= new Command(lang->CMDNAME_END_STR.getText());
	CMD_END_DBL		= new Command(lang->CMDNAME_END_DBL.getText());
	CMD_DISPLAY_STR	= new Command(lang->CMDNAME_DISPLAY_STR.getText());
	CMD_DISPLAY_DBL	= new Command(lang->CMDNAME_DISPLAY_DBL.getText());
	CMD_OPEN_INPUT	= new Command(lang->CMDNAME_OPEN_INPUT.getText());
	CMD_CLOSE_INPUT	= new Command(lang->CMDNAME_CLOSE_INPUT.getText());
	CMD_WRITE		= new Command(lang->CMDNAME_WRITE.getText());
	CMD_OPEN_OUTPUT	= new Command(lang->CMDNAME_OPEN_OUTPUT.getText());
	CMD_CLOSE_OUTPUT	= new Command(lang->CMDNAME_CLOSE_OUTPUT.getText());

	//Add built-in commands
	CMD_END_NULL->builtIn = true;
	add(new CommandRecord(CMD_END_NULL));
	
	CMD_END_STR->argTypeList = new vector<DataType>();
	CMD_END_STR->addArg(DT_STRING);
	CMD_END_STR->builtIn = true;
	add(new CommandRecord(CMD_END_STR));
	
	CMD_END_DBL->argTypeList = new vector<DataType>();
	CMD_END_DBL->addArg(DT_DOUBLE);
	CMD_END_DBL->builtIn = true;
	add(new CommandRecord(CMD_END_DBL));

	CMD_DISPLAY_STR->argTypeList = new vector<DataType>();
	CMD_DISPLAY_STR->addArg(DT_STRING);
	CMD_DISPLAY_STR->builtIn = true;
	add(new CommandRecord(CMD_DISPLAY_STR));

	CMD_DISPLAY_DBL->argTypeList = new vector<DataType>();
	CMD_DISPLAY_DBL->addArg(DT_DOUBLE);
	CMD_DISPLAY_DBL->builtIn = true;
	add(new CommandRecord(CMD_DISPLAY_DBL));

	CMD_OPEN_INPUT->argTypeList = new vector<DataType>();
	CMD_OPEN_INPUT->addArg(DT_STRING);
	CMD_OPEN_INPUT->builtIn = true;
	add(new CommandRecord(CMD_OPEN_INPUT));

	CMD_CLOSE_INPUT->builtIn = true;
	add(new CommandRecord(CMD_CLOSE_INPUT));

	CMD_WRITE->argTypeList = new vector<DataType>();
	CMD_WRITE->addArg(DT_STRING);
	CMD_WRITE->builtIn = true;
	add(new CommandRecord(CMD_WRITE));

	CMD_OPEN_OUTPUT->argTypeList = new vector<DataType>();
	CMD_OPEN_OUTPUT->addArg(DT_STRING);
	CMD_OPEN_OUTPUT->builtIn = true;
	add(new CommandRecord(CMD_OPEN_OUTPUT));

	CMD_CLOSE_OUTPUT->builtIn = true;
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

	FUNCT_TO_STRING		= new Function(DT_DOUBLE, lang->FUNCTNAME_TO_STRING.getText(), DT_STRING);
	FUNCT_TO_ASCII			= new Function(DT_DOUBLE, lang->FUNCTNAME_TO_ASCII.getText(), DT_STRING);
	FUNCT_PARSE_DOUBLE		= new Function(DT_STRING, lang->FUNCTNAME_PARSE_DOUBLE.getText(), DT_DOUBLE);
	FUNCT_ASCII_AT			= new Function(DT_STRING, lang->FUNCTNAME_ASCII_AT.getText(), DT_DOUBLE);
	FUNCT_LENGTH			= new Function(DT_STRING, lang->FUNCTNAME_LENGTH.getText(), DT_DOUBLE);
	FUNCT_RETRIEVE_DOUBLE	= new Function(DT_NULL, lang->FUNCTNAME_RETRIEVE_DOUBLE.getText(), DT_DOUBLE);
	FUNCT_RANDOM			= new Function(DT_NULL, lang->FUNCTNAME_RANDOM.getText(), DT_DOUBLE);
	FUNCT_RETRIEVE_STRING	= new Function(DT_NULL, lang->FUNCTNAME_RETRIEVE_STRING.getText(), DT_STRING);
	FUNCT_READ				= new Function(DT_NULL, lang->FUNCTNAME_READ.getText(), DT_STRING);
	FUNCT_ELEMENT_AT		= new Function(DT_ARGS, lang->FUNCTNAME_ELEMENT_AT.getText(), DT_STRING);
	FUNCT_SIZE				= new Function(DT_ARGS, lang->FUNCTNAME_SIZE.getText(), DT_DOUBLE);

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

	FUNCT_LENGTH->builtIn = true;
	add(new FunctionRecord(FUNCT_LENGTH));

	FUNCT_RETRIEVE_DOUBLE->builtIn = true;
	add(new FunctionRecord(FUNCT_RETRIEVE_DOUBLE));

	FUNCT_RANDOM->builtIn = true;
	add(new FunctionRecord(FUNCT_RANDOM));

	FUNCT_RETRIEVE_STRING->builtIn = true;
	add(new FunctionRecord(FUNCT_RETRIEVE_STRING));

	FUNCT_READ->builtIn = true;
	add(new FunctionRecord(FUNCT_READ));

	FUNCT_ELEMENT_AT->argTypeList = new vector<DataType>();
	FUNCT_ELEMENT_AT->addArg(DT_DOUBLE);
	FUNCT_ELEMENT_AT->builtIn = true;
	add(new FunctionRecord(FUNCT_ELEMENT_AT));

	FUNCT_SIZE->builtIn = true;
	add(new FunctionRecord(FUNCT_SIZE));
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