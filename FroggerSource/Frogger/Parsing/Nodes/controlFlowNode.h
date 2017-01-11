//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides a node for a control flow structure.
// -----------------------------------------------------------------
#pragma once

#include "node.h"
using namespace std;

class AbstractNode;
class Phase;

class BinaryOpNode;
class StmtNode;

// structure to generate an IfNode
struct IfStruct
{
	BinaryOpNode * boolExp;
	StmtNode * trueStmt;
	StmtNode * falseStmt;
};

// ----------------------------------------------------------
// This class provides a node for a control flow structure.
//
// Version 2.0
// ----------------------------------------------------------
class ControlFlowNode : public Node
{
protected: 
	ControlFlowNode * nextStmt; // the next statement linearly
	int stmtNo; // an identifier for this statement 

public: 
	// ----------------------------------------------------------
	// This function returns the next statement of code.
	//
	// Version 2.0
	// ----------------------------------------------------------
	ControlFlowNode * getNextStmt() { return nextStmt; }

	// ----------------------------------------------------------
	// This function returns this statement's identifier.
	//
	// Version 2.0
	// ----------------------------------------------------------
	int	 getStmtNo() { return stmtNo; }

	virtual void addStmt(AbstractNode * Stmt)=0;
	virtual void addIf(IfStruct ifStruct)=0;
	virtual void clean()=0;
	virtual void printNodes(ostream* out)=0;
	virtual void traverseNodes(Phase* p)=0;
};