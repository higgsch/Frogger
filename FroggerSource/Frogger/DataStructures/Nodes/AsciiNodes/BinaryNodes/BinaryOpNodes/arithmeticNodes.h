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
// Version 3.0
// ----------------------------------------------------------
class AddingNode : public BinaryOpNode
{
public:
	AddingNode() {}
	~AddingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a subtraction
// operation.
//
// Version 3.0
// ----------------------------------------------------------
class SubingNode : public BinaryOpNode
{
public:
	SubingNode() {}
	~SubingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a multiplication
// operation.
//
// Version 3.0
// ----------------------------------------------------------
class MulingNode : public BinaryOpNode
{
public:
	MulingNode(){}
	~MulingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a division
// operation.
//
// Version 3.0
// ----------------------------------------------------------
class DivingNode : public BinaryOpNode
{
public:
	DivingNode() {}
	~DivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a modulus 
// division operation.
//
// Version 3.0
// ----------------------------------------------------------
class ModDivingNode : public BinaryOpNode
{
public:
	ModDivingNode() {}
	~ModDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};
// ----------------------------------------------------------
// This class provides a node representation for an integer 
// division operation.
//
// Version 3.0
// ----------------------------------------------------------

class IDivingNode : public BinaryOpNode
{
public:
	IDivingNode() {}
	~IDivingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for a rootation 
// operation.
//
// Version 3.0
// ----------------------------------------------------------
class RootingNode : public BinaryOpNode
{
public:
	RootingNode() {}
	~RootingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};

// ----------------------------------------------------------
// This class provides a node representation for an exponentiation 
// operation.
//
// Version 3.0
// ----------------------------------------------------------
class ExpingNode : public BinaryOpNode
{
public:
	ExpingNode() {}
	~ExpingNode() {}
	
	void accept(Phase* p) { p->visit(this); }
};