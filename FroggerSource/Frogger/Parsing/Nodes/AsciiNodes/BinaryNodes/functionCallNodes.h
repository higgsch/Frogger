// -----------------------------------------------------------------
// This is the header file for function call classes. 
// -----------------------------------------------------------------
#pragma once

#include <vector>
#include "binaryNode.h"
#include "commandNodes.h"
#include "..\..\..\..\Phases\records.h"
using namespace std;

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