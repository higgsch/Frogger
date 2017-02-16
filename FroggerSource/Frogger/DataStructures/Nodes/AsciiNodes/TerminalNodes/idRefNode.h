// -----------------------------------------------------------------
// This is the header file for the IdRefNode class. 
// -----------------------------------------------------------------
#pragma once

#include "terminalNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a variable
// reference.
//
// Version 3.0
// ----------------------------------------------------------
class IdRefNode : public TerminalNode
{
public:
	IdRefNode(string id) { lexeme = id; }
	~IdRefNode() {}

	void accept(Phase* p) { p->visit(this); }
};