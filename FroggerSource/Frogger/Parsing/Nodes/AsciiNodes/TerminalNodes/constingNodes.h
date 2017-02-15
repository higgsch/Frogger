// -----------------------------------------------------------------
// This is the header file for the consting classes. 
// -----------------------------------------------------------------
#pragma once

#include "terminalNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a string
// constant.
//
// Version 3.0
// ----------------------------------------------------------
class StringConstingNode : public TerminalNode
{
public:
	StringConstingNode(string str) { lexeme = str; }
	~StringConstingNode() {}

	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a double
// constant.
//
// Version 3.0
// ----------------------------------------------------------
class DoubleConstingNode : public TerminalNode
{
public:
	DoubleConstingNode(string dbl) { lexeme = dbl; }
	~DoubleConstingNode() {}

	void accept(Phase* p)	{	p->visit(this);	}
};