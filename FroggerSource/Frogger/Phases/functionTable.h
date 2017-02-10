// -----------------------------------------------------------------
// This is the header for the FunctionTable class.
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "../Parsing/Nodes/abstractNode.h"
#include "../Parsing/Nodes/AbstractNodeChild/functionCallNodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a collection of functions
//
// Version 2.4
// ----------------------------------------------------------
class FunctionTable
{
private:
	vector<Function*> * table;

public:
	FunctionTable();
	void addFunction(Function * funct);
	bool functionDefined(Function * funct);
	bool matchExists(Function * funct);
	int getNumberOfMatches(Function * funct);
	Function* getFirstMatch(Function * funct);
	DataType getFunctionReturnType(Function * funct);
};