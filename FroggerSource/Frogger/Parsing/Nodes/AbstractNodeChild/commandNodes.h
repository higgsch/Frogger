// -----------------------------------------------------------------
// This is the header file for the command node classes. 
// -----------------------------------------------------------------
#pragma once

#include "..\abstractNode.h"
#include <vector>
using namespace std;

// ----------------------------------------------------------
// This class represents the information known about a command.
// It holds known information about an individual command.
//
// Version 2.5
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

	int getNumArgs() { return argTypeList->size(); }
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
// This class provides a node representation for a command call.
//
// Version 2.5
// ----------------------------------------------------------
class CommandCallNode : public AbstractNode
{
private:
	Command * cmd;

public:
	CommandCallNode(string);

	// ----------------------------------------------------------
	// This function prints this node to the given output stream.
	// @out: The stream to display to.
	//
	// Version 2.5
	// ----------------------------------------------------------
	void printMe(ostream* out)	
	{	
		*out << lexeme << "(";

		if (rightChild != NULL)
		{
			rightChild->printMe(out);
		}

		*out << ")";
	}

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.5
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}

	Command* getCmd() { return cmd; }
	void setCmd(Command * command)
	{
		//Don't just change pointer because ArgListNodes use the old pointer
		cmd->copy(command);
	}
	int getArgListLength() { return cmd->getNumArgs(); }
};

// ----------------------------------------------------------
// This class provides a node representation for an element
// in an argument list.
//
// Version 2.5
// ----------------------------------------------------------
class ArgListNode : public AbstractNode
{
private:
	Command * cmd;
	int argNo;

public:
	ArgListNode();
	void printMe(ostream*);

	// ----------------------------------------------------------
	// This function allows double dispatch required for visitor
	// pattern.
	// @p: The visitor to operate on this node.
	//
	// Version 2.4
	// ----------------------------------------------------------
	void accept(Phase* p)	{	p->visit(this);	}

	void setArgNo(int i) { argNo = i; }
	int getArgNo() { return argNo; }
	void setCmd(Command* c) { cmd = c; }
	Command* getCmd() { return cmd; }
	bool isListTyped(){	return isTreeTyped(); }
};