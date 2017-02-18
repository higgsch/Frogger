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
// Version 3.1
// ----------------------------------------------------------
class AddingNode : public BinaryOpNode
{
public:
	AddingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~AddingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a subtraction
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class SubingNode : public BinaryOpNode
{
public:
	SubingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~SubingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a multiplication
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class MulingNode : public BinaryOpNode
{
public:
	MulingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~MulingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a division
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class DivingNode : public BinaryOpNode
{
public:
	DivingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~DivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a modulus 
// division operation.
//
// Version 3.1
// ----------------------------------------------------------
class ModDivingNode : public BinaryOpNode
{
public:
	ModDivingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~ModDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};
// ----------------------------------------------------------
// This class provides a node representation for an integer 
// division operation.
//
// Version 3.1
// ----------------------------------------------------------

class IDivingNode : public BinaryOpNode
{
public:
	IDivingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~IDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a rootation 
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class RootingNode : public BinaryOpNode
{
public:
	RootingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~RootingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for an exponentiation 
// operation.
//
// Version 3.1
// ----------------------------------------------------------
class ExpingNode : public BinaryOpNode
{
public:
	ExpingNode(int lineNo) : BinaryOpNode(lineNo) {}
	~ExpingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};