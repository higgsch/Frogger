// -----------------------------------------------------------------
// This is the header file for function call classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
#include <vector>
using namespace std;

// ----------------------------------------------------------
// This class represents the information known about a Function.
// It holds known information about an individual function.
//
// Version 2.4
// ----------------------------------------------------------
class Function
{
public:
	bool builtIn;
	DataType parentType;
	string name;
	DataType returnType;
	vector<DataType> *argTypeList;

	Function(DataType i_parentType, string i_name, DataType i_returnType);
	void addArg(DataType arg);
	bool equals(Function* funct);
	bool matches(Function* funct);
	void copy(Function* funct);

	int getNumArgs() { return argTypeList->size(); }
	DataType getDataTypeOfArgNumber(int argNo) 
	{ 
		if (argTypeList == NULL)
			return DT_NOT_DEFINED;
		else
			return argTypeList->at(argNo);
	}
	void setDataTypeOfArgNumber(int argNo, DataType type) 
	{ 
		if (argNo < argTypeList->size()) 
			(*argTypeList)[argNo] = type; 
	}
	bool isUserFunction() { return !builtIn; }
};

// ----------------------------------------------------------
// This class provides a node representation for a function
// call.
//
// Version 2.4
// ----------------------------------------------------------
class FunctionCallNode : public AbstractNode
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
	int getArgListLength() { return funct->getNumArgs(); }
};

// ----------------------------------------------------------
// This class provides a node representation for an element
// in an argument list.
//
// Version 2.4
// ----------------------------------------------------------
class ArgListNode : public AbstractNode
{
private:
	Function * funct;
	int argNo;

public:
	ArgListNode();
	void printMe(ostream*);

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.4
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}

	void setArgNo(int i) { argNo = i; }
	int getArgNo() { return argNo; }
	void setFunct(Function* f) { funct = f; }
	Function* getFunct() { return funct; }
	bool isListTyped(){	return isTreeTyped(); }
};