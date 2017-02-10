//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.4
// ----------------------------------------------------------------
// This program represents a collection of variables
// -----------------------------------------------------------------
#include "functionTable.h"
using namespace std;

// ----------------------------------------------------------
// This is the default constructor for a FunctionTable.
//
// Version 2.4
// ----------------------------------------------------------
FunctionTable::FunctionTable()
{
	table = new vector<Function*>();

	//Add built-in functions
	Function * toString = new Function(DT_DOUBLE,"toString",DT_STRING);
	toString->builtIn = true;
	table->push_back(toString);

	Function * toAscii = new Function(DT_DOUBLE,"toAscii",DT_STRING);
	toAscii->builtIn = true;
	table->push_back(toAscii);

	Function * parseDouble = new Function(DT_STRING,"parseDouble",DT_DOUBLE);
	parseDouble->builtIn = true;
	table->push_back(parseDouble);

	Function * asciiAt = new Function(DT_STRING,"asciiAt",DT_DOUBLE);
	asciiAt->argTypeList = new vector<DataType>();
	asciiAt->addArg(DT_DOUBLE);
	asciiAt->builtIn = true;
	table->push_back(asciiAt);
}

// ----------------------------------------------------------
// This function adds a function to the table.
// @funct: The function to add.
//
// Version 2.4
// ----------------------------------------------------------
void FunctionTable::addFunction(Function * funct)
{
	if (funct->parentType == DT_NOT_DEFINED)
		return;

	if (funct->returnType == DT_NOT_DEFINED)
		return;

	for (DataType t : *(funct->argTypeList))
	{
		if (t == DT_NOT_DEFINED)
			return;
	}

	if (!functionDefined(funct))
		table->push_back(funct);
}

// ----------------------------------------------------------
// This function determines if the function already exists in 
// the table.
// @funct: The function in question.
//
// Version 2.4
// ----------------------------------------------------------
bool FunctionTable::functionDefined(Function * funct)
{
	for (Function* f : *table)
	{
		if (f->equals(funct))
			return true;
	}
	return false;
}

// ----------------------------------------------------------
// This function determines if the function matches a defined
// function
// @funct: The function in question.
//
// Version 2.4
// ----------------------------------------------------------
bool FunctionTable::matchExists(Function * funct)
{
	for (Function* f : *table)
	{
		if (f->matches(funct))
			return true;
	}
	return false;
}

// ----------------------------------------------------------
// This function returns the number of defined functions that
// match the desired function.
// @funct: The function in question.
//
// Version 2.4
// ----------------------------------------------------------
int FunctionTable::getNumberOfMatches(Function * funct)
{
	int matches = 0;
	for (Function* f : *table)
	{
		if (f->matches(funct))
			matches++;
	}
	return matches;
}

// ----------------------------------------------------------
// This function returns the first matching function in the 
// table or NULL if no match found.
// @funct: The function in question.
//
// Version 2.4
// ----------------------------------------------------------
Function * FunctionTable::getFirstMatch(Function * funct)
{
	for (Function* f : *table)
	{
		if (f->matches(funct))
			return f;
	}
	return NULL;
}

// ----------------------------------------------------------
// This function returns the return type of the desired 
// function or DataType::DT_NOT_DEFINED if the function is not 
// in the table.
// @funct: The function in question.
//
// Version 2.4
// ----------------------------------------------------------
DataType FunctionTable::getFunctionReturnType(Function * funct)
{
	for (Function* f : *table)
	{
		if (f->equals(funct))
			return f->returnType;
	}

	return DT_NOT_DEFINED;
}