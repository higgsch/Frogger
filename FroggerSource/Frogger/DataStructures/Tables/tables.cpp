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
	Command * end = new Command("end");
	end->builtIn = true;
	add(new CommandRecord(end));

	Command * displayString = new Command("display");
	displayString->argTypeList = new vector<DataType>();
	displayString->addArg(DT_STRING);
	displayString->builtIn = true;
	add(new CommandRecord(displayString));

	Command * displayDouble = new Command("display");
	displayDouble->argTypeList = new vector<DataType>();
	displayDouble->addArg(DT_DOUBLE);
	displayDouble->builtIn = true;
	add(new CommandRecord(displayDouble));
}

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 3.0
// ----------------------------------------------------------
FunctionTable::FunctionTable()
{
	//Add built-in functions
	Function * toString = new Function(DT_DOUBLE,"toString",DT_STRING);
	toString->builtIn = true;
	add(new FunctionRecord(toString));

	Function * toAscii = new Function(DT_DOUBLE,"toAscii",DT_STRING);
	toAscii->builtIn = true;
	add(new FunctionRecord(toAscii));

	Function * parseDouble = new Function(DT_STRING,"parseDouble",DT_DOUBLE);
	parseDouble->builtIn = true;
	add(new FunctionRecord(parseDouble));

	Function * asciiAt = new Function(DT_STRING,"asciiAt",DT_DOUBLE);
	asciiAt->argTypeList = new vector<DataType>();
	asciiAt->addArg(DT_DOUBLE);
	asciiAt->builtIn = true;
	add(new FunctionRecord(asciiAt));

	Function * retrieveDouble = new Function(DT_NULL, "retrieveDouble", DT_DOUBLE);
	retrieveDouble->builtIn = true;
	add(new FunctionRecord(retrieveDouble));

	Function * random = new Function(DT_NULL, "random", DT_DOUBLE);
	random->builtIn = true;
	add(new FunctionRecord(random));

	Function * retrieveString = new Function(DT_NULL, "retrieveString", DT_STRING);
	retrieveString->builtIn = true;
	add(new FunctionRecord(retrieveString));
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