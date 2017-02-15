// -----------------------------------------------------------------
// This is the header file for the command node classes. 
// -----------------------------------------------------------------
#pragma once

#include "binaryNode.h"
#include <vector>
using namespace std;

// ----------------------------------------------------------
// This class represents the information known about a command.
// It holds known information about an individual command.
//
// Version 3.0
// ----------------------------------------------------------
class Command
{
public:
	bool builtIn;
	string name;
	vector<DataType> *argTypeList;

	Command(string i_name);
	void addArg(DataType arg);
	bool equals(Command* cmd);
	bool matches(Command* cmd);
	void copy(Command* cmd);

	int getNumArgs() { 
		if (argTypeList != NULL)
			return argTypeList->size(); 
		else
			return 0;
	}
	DataType getDataTypeOfArgNumber(int argNo) 
	{ 
		if (argTypeList == NULL)
			return DT_NOT_DEFINED;
		else
			return argTypeList->at(argNo);
	}
	void setDataTypeOfArgNumber(int argNo, DataType type) 
	{ 
		if (argNo < argTypeList->size()) 
			(*argTypeList)[argNo] = type; 
	}
	bool isUserDefined() { return !builtIn; }
};

// ----------------------------------------------------------
// This class provides a node representation for an element
// in an argument list.
//
// Version 3.0
// ----------------------------------------------------------
class ArgListNode : public BinaryNode
{
private:
	Command * cmd;
	int argNo;

public:
	ArgListNode() { cmd = NULL; argNo = -1;}
	~ArgListNode() {} // cmd is deleted by FunctionCallNode or CommandCallNode 

	void setArgNo(int i) { argNo = i; }
	int getArgNo() { return argNo; }
	void setCmd(Command* c) { cmd = c; }
	Command* getCmd() { return cmd; }
	bool isListTyped(){	return isTreeTyped(); }
	bool hasNextArg() { return getNextArg() != NULL; }

	void addThisArg(AsciiNode* n) { addLeftChild(n); }
	void addNextArg(AsciiNode* n) { addRightChild(n); }

	AsciiNode* getThisArg() { return getLeftChild(); }
	AsciiNode* getNextArg() { return getRightChild(); }
	
	void visitThisArg(Phase* p) { visitLeftChild(p); }
	void visitNextArg(Phase* p) { visitRightChild(p); }
	void accept(Phase* p)	{	p->visit(this);	}
};

// ----------------------------------------------------------
// This class provides a node representation for a command call.
//
// Version 3.0
// ----------------------------------------------------------
class CommandCallNode : public BinaryNode
{
private:
	Command * cmd;

public:
	CommandCallNode(string);
	~CommandCallNode();

	Command* getCmd() { return cmd; }
	//Don't just change pointer because ArgListNodes use the old pointer
	void setCmd(Command * command) { cmd->copy(command); }
	int getArgListLength() { return cmd->getNumArgs(); }

	void addPrimary(AsciiNode* n) { addLeftChild(n); }
	void addArgList(AsciiNode* n) { addRightChild(n); }

	AsciiNode* getPrimary() { return getLeftChild(); }
	AsciiNode* getArgList() { return getRightChild(); }

	void visitPrimary(Phase* p) { visitLeftChild(p); }
	void visitArgList(Phase* p) { visitRightChild(p); }
	void accept(Phase* p)	{	p->visit(this);	}
};