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
// Version 3.0
// ----------------------------------------------------------
class NotingNode : public UnaryNode
{
public:
	NotingNode() {}
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
// Version 3.0
// ----------------------------------------------------------
class LTingNode : public BinaryOpNode
{
public:
	LTingNode()	{}
	~LTingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than comparison.
//
// Version 3.0
// ----------------------------------------------------------
class GTingNode : public BinaryOpNode
{
public:
	GTingNode() {}
	~GTingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for an equivalence
// comparison.
//
// Version 3.0
// ----------------------------------------------------------
class EQingNode : public BinaryOpNode
{
public:
	EQingNode() {}
	~EQingNode() {}
	
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a less than
// or equal comparison.
//
// Version 3.0
// ----------------------------------------------------------
class LTEingNode : public BinaryOpNode
{
public:
	LTEingNode() {}
	~LTEingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a greater 
// than or equal comparison.
//
// Version 3.0
// ----------------------------------------------------------
class GTEingNode : public BinaryOpNode
{
public:
	GTEingNode() {}
	~GTEingNode() {}

	void accept(Phase* p) { p->visit(this); }
};