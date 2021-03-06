// -----------------------------------------------------------------
// This is the header file for function call classes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryNode.h"
#include "commandNodes.h"
#include "..\..\..\Tables\records.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a function
// call.
//
// Version 4.2
// ----------------------------------------------------------
class FunctionCallNode : public CommandCallNode
{
private:
	Function * funct;

public:
	FunctionCallNode(string name, int lineNo);
	~FunctionCallNode();
	
	Function* getFunct() { return funct; }
	//Don't just change pointer because ArgListNodes use the old pointer
	void setFunct(Function * function) { funct->copy(function); }

	int getArgListLength() { return funct->getNumArgs(); }

	void accept(Phase* p) { p->visit(this); }
};