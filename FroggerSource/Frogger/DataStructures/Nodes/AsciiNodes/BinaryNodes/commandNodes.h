// -----------------------------------------------------------------
// This is the header file for the command node classes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryNode.h"
#include "..\..\..\Tables\records.h"
using namespace std;

// ----------------------------------------------------------
// This class provides a node representation for an element
// in an argument list.
//
// Version 4.2
// ----------------------------------------------------------
class ArgListNode : public BinaryNode
{
private:
	Routine * rout;
	int argNo;

public:
	ArgListNode(int lineNo) : BinaryNode(lineNo) { rout = NULL; argNo = -1;}
	~ArgListNode() {} // cmd is deleted by FunctionCallNode or CommandCallNode 

	void setArgNo(int i) { argNo = i; }
	int getArgNo() { return argNo; }
	void setRoutine(Routine* r) { rout = r; }
	Routine* getRoutine() { return rout; }
	bool isListTyped(){	return isTreeTyped(); }
	bool hasNextArg() { return getNextArg() != NULL; }

	void addThisArg(AsciiNode* n) { addLeftChild(n); }
	void addNextArg(AsciiNode* n) { addRightChild(n); }

	AsciiNode* getThisArg() { return getLeftChild(); }
	AsciiNode* getNextArg() { return getRightChild(); }
	
	void visitThisArg(FGRPhase* p) { visitLeftChild(p); }
	void visitNextArg(FGRPhase* p) { visitRightChild(p); }
	void accept(FGRPhase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a command call.
//
// Version 5.0
// ----------------------------------------------------------
class CommandCallNode : public BinaryNode
{
private:
	Routine * cmd;

public:
	CommandCallNode(DataType * primary, string name, int lineNo);
	~CommandCallNode();

	Routine* getCmd() { return cmd; }
	//Don't just change pointer because ArgListNodes use the old pointer
	void setCmd(Routine * command) { cmd->copy(command); }
	int getArgListLength() { return cmd->getNumArgs(); }

	void addPrimary(AsciiNode* n) { addLeftChild(n); }
	void addArgList(AsciiNode* n) { addRightChild(n); }

	AsciiNode* getPrimary() { return getLeftChild(); }
	AsciiNode* getArgList() { return getRightChild(); }

	void visitPrimary(FGRPhase* p) { visitLeftChild(p); }
	void visitArgList(FGRPhase* p) { visitRightChild(p); }
	void accept(FGRPhase* p)	{	p->visit(this);	}
};