// -----------------------------------------------------------------
// This is the header file for the UnaryNode class. 
// -----------------------------------------------------------------
#pragma once

#include "asciiNode.h"
using namespace std;

class FGRPhase;

// ----------------------------------------------------------
// This class provides representation for nodes with one 
// child.
//
// Version 3.1
// ----------------------------------------------------------
class UnaryNode : public AsciiNode
{
private:
	AsciiNode * child; // this node's only child

protected:
	void addChild(AsciiNode * absNode);
	void visitChild(FGRPhase* p) { visitNode(p, child); }

public:
	UnaryNode(int lineNo) : AsciiNode(lineNo) { child = NULL; }
	~UnaryNode();
	
	AsciiNode * getChild() { return child; }

	bool isTreeTyped();

	void visitAllChildren(FGRPhase* p) { visitChild(p); }
};