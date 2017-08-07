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
// Version 5.0
// ----------------------------------------------------------
class FunctionCallNode : public CommandCallNode
{
private:
	Routine * funct;

public:
	FunctionCallNode(string name, int lineNo);
	~FunctionCallNode();
	
	Routine* getFunct() { return funct; }
	//Don't just change pointer because ArgListNodes use the old pointer
	void setFunct(Routine * function) { funct->copy(function); }

	int getArgListLength() { return funct->getNumArgs(); }

	void accept(Phase* p) { p->visit(this); }
};