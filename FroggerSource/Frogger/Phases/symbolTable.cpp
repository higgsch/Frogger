//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// ----------------------------------------------------------------
// This program represents a collection of variables
// -----------------------------------------------------------------
#include "symbolTable.h"
#include <string>
using namespace std;

// ----------------------------------------------------------
// This function adds a symbol to the table.
// @id: The string representation of the variable.
// @type: The data type of the variable.
//
// Version 2.3
// ----------------------------------------------------------
void SymbolTable::addSymbol(string id, DataType type)
{
	if (type == DT_NOT_DEFINED || symbolDefined(id))
		return;

	table->push_back(new Symbol(id,type));
}

// ----------------------------------------------------------
// This function determines if the variable already exists in 
// the table.
// @id: The string representation of the variable in question.
//
// Version 2.3
// ----------------------------------------------------------
bool SymbolTable::symbolDefined(string id)
{
	for (Symbol* s : *table)
	{
		if (s->id == id)
			return true;
	}

	return false;
}

// ----------------------------------------------------------
// This function returns the data type of the desired symbol
// or DataType::DT_NOT_DEFINED if the symbol is not in the 
// table.
// @id: The string representation of the variable in question.
//
// Version 2.3
// ----------------------------------------------------------
DataType SymbolTable::symbolType(string id)
{
	for (Symbol* s : *table)
	{
		if (s->id == id)
			return s->type;
	}

	return DT_NOT_DEFINED;
}