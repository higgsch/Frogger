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
class JmpStmtNode;

// structure to generate an IfNode
struct IfStruct
{
	BinaryOpNode * boolExp;
	JmpStmtNode * trueStmt;
	JmpStmtNode * falseStmt;
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
	bool isNestedStmt; // an indicator for nested statements
		// if true, nextStmt should be NULL and stmtNo is N/A.

public: 
	// ----------------------------------------------------------
	// Default constructor.
	//
	// Version 2.0
	// ----------------------------------------------------------
	ControlFlowNode() { isNestedStmt = false; }

	// ----------------------------------------------------------
	// This function returns the next statement of code.
	//
	// Version 2.0
	// ----------------------------------------------------------
	ControlFlowNode * getNextStmt() { return (isNestedStmt) ? NULL : nextStmt; }

	// ----------------------------------------------------------
	// This function returns this statement's identifier.
	//
	// Version 2.0
	// ----------------------------------------------------------
	int	 getStmtNo() { return (isNestedStmt) ? -1 : stmtNo; }

	// ----------------------------------------------------------
	// This function returns whether this statement is nested.
	//
	// Version 2.0
	// ----------------------------------------------------------
	bool isNested() { return isNestedStmt; }

	// ----------------------------------------------------------
	// This function sets this statement as nested or not.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void setNested(bool nest) { isNestedStmt = nest; }

	// ----------------------------------------------------------
	// This function sets the statement number.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void setStmtNo(int num) { stmtNo = num; }

	virtual void accept(Phase*)=0;

	virtual void addNextStmt(ControlFlowNode* next)=0;
	virtual void clean()=0;
	virtual void printNodes(ostream* out)=0;
};