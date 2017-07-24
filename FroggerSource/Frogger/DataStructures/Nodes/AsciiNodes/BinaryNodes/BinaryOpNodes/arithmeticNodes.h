// -----------------------------------------------------------------
// This is the header file for the arithmetic nodes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryOpNode.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for an addition
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class AddingNode : public BinaryOpNode
{
public:
	AddingNode(int lineNo) : BinaryOpNode(lineNo, ADD) {}
	~AddingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a subtraction
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class SubingNode : public BinaryOpNode
{
public:
	SubingNode(int lineNo) : BinaryOpNode(lineNo, SUB) {}
	~SubingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a multiplication
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class MulingNode : public BinaryOpNode
{
public:
	MulingNode(int lineNo) : BinaryOpNode(lineNo, MUL) {}
	~MulingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a division
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class DivingNode : public BinaryOpNode
{
public:
	DivingNode(int lineNo) : BinaryOpNode(lineNo, DIV) {}
	~DivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a modulus 
// division operation.
//
// Version 4.2
// ----------------------------------------------------------
class ModDivingNode : public BinaryOpNode
{
public:
	ModDivingNode(int lineNo) : BinaryOpNode(lineNo, MODDIV) {}
	~ModDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};
// ----------------------------------------------------------
// This class provides a node representation for an integer 
// division operation.
//
// Version 4.2
// ----------------------------------------------------------

class IDivingNode : public BinaryOpNode
{
public:
	IDivingNode(int lineNo) : BinaryOpNode(lineNo, IDIV) {}
	~IDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a rootation 
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class RootingNode : public BinaryOpNode
{
public:
	RootingNode(int lineNo) : BinaryOpNode(lineNo, ROOT) {}
	~RootingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for an exponentiation 
// operation.
//
// Version 4.2
// ----------------------------------------------------------
class ExpingNode : public BinaryOpNode
{
public:
	ExpingNode(int lineNo) : BinaryOpNode(lineNo, EXP) {}
	~ExpingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};