// -----------------------------------------------------------------
// This is the header file for function call classes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryNode.h"
#include "commandNodes.h"
#include <vector>
using namespace std;

// ----------------------------------------------------------
// This class represents the information known about a Function.
// It holds known information about an individual function.
//
// Version 3.0
// ----------------------------------------------------------
class Function : public Command
{
public:
	DataType parentType;
	DataType returnType;

	Function(DataType i_parentType, string i_name, DataType i_returnType);
	bool equals(Function* funct);
	bool matches(Function* funct);
	void copy(Function* funct);
};

// ----------------------------------------------------------
// This class provides a node representation for a function
// call.
//
// Version 3.0
// ----------------------------------------------------------
class FunctionCallNode : public CommandCallNode
{
private:
	Function * funct;

public:
	FunctionCallNode(string);
	~FunctionCallNode();
	
	Function* getFunct() { return funct; }
	//Don't just change pointer because ArgListNodes use the old pointer
	void setFunct(Function * function) { funct->copy(function); }

	void accept(Phase* p) { p->visit(this); }
};