// -----------------------------------------------------------------
// This is the header file for function call classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
#include "commandNodes.h"
#include <vector>
using namespace std;

// ----------------------------------------------------------
// This class represents the information known about a Function.
// It holds known information about an individual function.
//
// Version 2.5
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
// Version 2.5
// ----------------------------------------------------------
class FunctionCallNode : public CommandCallNode
{
private:
	Function * funct;

public:
	FunctionCallNode(string);
	void printMe(ostream* out);

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.4
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}
	
	Function* getFunct() { return funct; }
	void setFunct(Function * function)
	{
		//Don't just change pointer because ArgListNodes use the old pointer
		funct->copy(function);
	}
};