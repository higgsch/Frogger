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
// Version 3.1
// ----------------------------------------------------------
class StringConstingNode : public TerminalNode
{
public:
	StringConstingNode(string str, int lineNo) : TerminalNode(lineNo) { lexeme = str; }
	~StringConstingNode() {}

	void accept(FGRPhase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a double
// constant.
//
// Version 3.1
// ----------------------------------------------------------
class DoubleConstingNode : public TerminalNode
{
public:
	DoubleConstingNode(string dbl, int lineNo) : TerminalNode(lineNo) { lexeme = dbl; }
	~DoubleConstingNode() {}

	void accept(FGRPhase* p)	{	p->visit(this);	}
};