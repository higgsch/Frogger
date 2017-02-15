// -----------------------------------------------------------------
// This is the header for the SymbolTable class.
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "..\Parsing\Nodes\AsciiNodes\asciiNode.h"
using namespace std;

// ----------------------------------------------------------
// This structure represents an entry in the SymbolTable class.
// It holds known information about an individual variable.
//
// Version 2.3
// ----------------------------------------------------------
struct Symbol
{
	string id;
	DataType type;

	Symbol(string i_id, DataType i_type) { id = i_id; type = i_type;}
};

// ----------------------------------------------------------
// This class represents a collection of variables
//
// Version 2.3
// ----------------------------------------------------------
class SymbolTable
{
private:
	vector<Symbol*> * table;

public:
	SymbolTable(){	table = new vector<Symbol*>();	}
	void addSymbol(string id, DataType type);
	bool symbolDefined(string id);
	DataType symbolType(string id);
};