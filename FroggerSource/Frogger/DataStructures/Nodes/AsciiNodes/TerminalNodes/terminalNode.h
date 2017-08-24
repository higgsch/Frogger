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
// Version 3.1
// ----------------------------------------------------------
class TerminalNode : public AsciiNode
{
public:
	TerminalNode(int lineNo) : AsciiNode(lineNo) {}
	~TerminalNode() {}

	bool isTreeTyped() { return isTyped(); }

	void visitAllChildren(FGRPhase* p) { return; }
};