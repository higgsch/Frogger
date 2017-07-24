// -----------------------------------------------------------------
// This is the header file for the boolean logic classes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryOpNode.h"
#include "..\..\unaryNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for a boolean not
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class NotingNode : public UnaryNode
{
public:
	NotingNode(int lineNo) : UnaryNode(lineNo) {}
	~NotingNode() {}

	void addOperand(AsciiNode* n) { addChild(n); }
	AsciiNode* getOperand() { return getChild(); }

	void visitOperand(Phase* p) { visitChild(p); }
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a less than
// comparison.
//
// Version 4.2
// ----------------------------------------------------------
class LTingNode : public BinaryOpNode
{
public:
	LTingNode(int lineNo) : BinaryOpNode(lineNo, LT) {}
	~LTingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than comparison.
//
// Version 4.2
// ----------------------------------------------------------
class GTingNode : public BinaryOpNode
{
public:
	GTingNode(int lineNo) : BinaryOpNode(lineNo, GT) {}
	~GTingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for an equivalence
// comparison.
//
// Version 4.2
// ----------------------------------------------------------
class EQingNode : public BinaryOpNode
{
public:
	EQingNode(int lineNo) : BinaryOpNode(lineNo, EQ) {}
	~EQingNode() {}
	
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a less than
// or equal comparison.
//
// Version 4.2
// ----------------------------------------------------------
class LTEingNode : public BinaryOpNode
{
public:
	LTEingNode(int lineNo) : BinaryOpNode(lineNo, LTE) {}
	~LTEingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than or equal comparison.
//
// Version 4.2
// ----------------------------------------------------------
class GTEingNode : public BinaryOpNode
{
public:
	GTEingNode(int lineNo) : BinaryOpNode(lineNo, GTE) {}
	~GTEingNode() {}

	void accept(Phase* p) { p->visit(this); }
};