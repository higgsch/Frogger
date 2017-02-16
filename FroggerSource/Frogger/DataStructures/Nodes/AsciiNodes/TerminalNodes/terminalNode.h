// -----------------------------------------------------------------
// This is the header file for the TerminalNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\asciiNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides representation for nodes with no
// children.
//
// Version 3.0
// ----------------------------------------------------------
class TerminalNode : public AsciiNode
{
public:
	TerminalNode() {}
	~TerminalNode() {}

	bool isTreeTyped() { return isTyped(); }

	void visitAllChildren(Phase* p) { return; }
};