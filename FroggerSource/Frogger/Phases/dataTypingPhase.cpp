//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.4
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// Default constructor.
//
// Version 4.4
// ----------------------------------------------------------
DataTypingPhase::DataTypingPhase(Language * language, FunctionTable * functs, CommandTable * cmds, SymbolTable * syms) 
{ 
	lang = language;

	symbols = syms; 
	functions = functs;
	commands = cmds;
	changeMadeThisRound = false; 
	setUnknownTypeNodesToDefault = false;
}

// ----------------------------------------------------------
// This function starts the phase on the program root node.
// @n: The node representing the program.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(ProgramNode * n)
{
	do
	{
		changeMadeThisRound = false;
		n->visitAllChildren(this);
	} while (changeMadeThisRound == true);

	setUnknownTypeNodesToDefault = true;
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 4.2
// ----------------------------------------------------------
void DataTypingPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	DataType type = n->getDataType();
	Symbol sym = Symbol(id, DT_NOT_DEFINED, true);

	if (type == DT_NOT_DEFINED)
	{
		if (symbols->isDefined(&sym))
		{
			n->setDataType(symbols->symbolType(id));
			return;
		}

		if (setUnknownTypeNodesToDefault)
			symbols->add(new SymbolRecord(id, DT_DOUBLE, true));

		return;
	}
	
	if (!symbols->isDefined(&sym))
		symbols->add(new SymbolRecord(id, type, true));
	else
	{
		if (symbols->symbolType(id) != type)
			dataType_error("Variable " + id + " used as multiple type", n->getLineNo());
	}
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AssigningNode * n)
{
	n->visitAllChildren(this);
	unifyTreeDataType(n);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 3.1
// ----------------------------------------------------------
void DataTypingPhase::visit(FunctionCallNode * n)
{
	DataType oldNodeType = n->getDataType();

	if (oldNodeType != DT_NOT_DEFINED)
		checkAndSetNodeDataType(n, oldNodeType);

	DataType oldFunctType = n->getFunct()->returnType;
	if (oldFunctType != DT_NOT_DEFINED)
		checkAndSetNodeDataType(n, oldFunctType);
	else if (oldNodeType != DT_NOT_DEFINED)
		n->getFunct()->returnType = oldNodeType;

	n->visitAllChildren(this);

	Function * funct = n->getFunct();
	if (!functions->matchExists(funct))
		dataType_error("Function call does not match signature: " + funct->name, n->getLineNo());
	else if (functions->getNumberOfMatches(funct) == 1)
	{
		n->setFunct(functions->getFirstMatch(funct));
		checkAndSetNodeDataType(n, funct->returnType);
		n->visitAllChildren(this);
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 3.1
// ----------------------------------------------------------
void DataTypingPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Command * cmd = n->getCmd();
	if (!commands->matchExists(cmd))
		dataType_error("Command call does not match signature: " + cmd->name, n->getLineNo());
	else if (commands->getNumberOfMatches(cmd) == 1)
	{
		n->setCmd(commands->getFirstMatch(cmd));
		n->visitAllChildren(this);
	}
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void DataTypingPhase::visit(ArgListNode * n)
{
	int argNo = n->getArgNo();
	DataType oldArgType = n->getRoutine()->getArg(argNo)->type;
	if (oldArgType != DT_NOT_DEFINED)
		checkAndSetNodeDataType(n->getThisArg(),oldArgType);

	n->visitAllChildren(this);
	
	DataType argType = n->getThisArg()->getDataType();
	if (argType != DT_NOT_DEFINED)
	{
		checkAndSetNodeDataType(n, argType);
		checkAndSetArgDataType(n->getRoutine(), n->getArgNo(), argType, n->getLineNo());
	}
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AddingNode * n)
{
	n->visitAllChildren(this);
	unifyTreeDataType(n);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node. It throws
// a dataType error if there is a dataType conflict.
// 
// Version 3.1
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetNodeDataType(AsciiNode * node, DataType type)
{
	if (node->getDataType() == type || type == DT_NOT_DEFINED)
		return;

	if (node->getDataType() == DT_NOT_DEFINED)
	{
		node->setDataType(type);
		changeMadeThisRound = true;
	}
	else
		dataType_error("Data Type Conflict", node->getLineNo());
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children. It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(TerminalNode * node, DataType type)
{
	if (type == DT_NOT_DEFINED)
		return;

	DataType thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children. It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(UnaryNode * node, DataType type)
{
	if (type == DT_NOT_DEFINED)
		return;

	DataType thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);

	AsciiNode *child = node->getChild();
	if (child != NULL)
	{
		DataType leftOldType = child->getDataType();
		checkAndSetNodeDataType(child, type);
		if (leftOldType == DT_NOT_DEFINED)
			child->accept(this);
	}

	if (thisOldType == DT_NOT_DEFINED)
		node->accept(this);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children. It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 2.5
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(BinaryNode * node, DataType type)
{
	if (type == DT_NOT_DEFINED)
		return;

	DataType thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);

	AsciiNode *left = node->getLeftChild();
	if (left != NULL)
	{
		DataType leftOldType = left->getDataType();
		checkAndSetNodeDataType(left, type);
		if (leftOldType == DT_NOT_DEFINED)
			left->accept(this);
	}

	AsciiNode *right = node->getRightChild();
	if (right != NULL)
	{
		DataType rightOldType = right->getDataType();
		checkAndSetNodeDataType(right, type);
		if (rightOldType == DT_NOT_DEFINED)
			right->accept(this);
	}

	if (thisOldType == DT_NOT_DEFINED)
		node->accept(this);
}

// ----------------------------------------------------------
// This function safely sets the dataType of an argument. 
// It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 4.2
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetArgDataType(Routine * rout, int argNo, DataType type, int lineNo)
{
	DataType oldType = rout->getArg(argNo)->type;
	if (oldType == type || type == DT_NOT_DEFINED)
		return;

	if (oldType == DT_NOT_DEFINED)
	{
		rout->setDataTypeOfArgNumber(argNo, type);
		return;
	}
	else
		dataType_error("Data Type Conflict", lineNo);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(TerminalNode * node)
{
	DataType type = node->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}
	
	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(UnaryNode * node)
{
	DataType type = node->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *child = node->getChild();
	type = child->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(BinaryNode * node)
{
	DataType type = node->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *left = node->getLeftChild();
	type = left->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *right = node->getRightChild();
	type = right->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @err_msg: The message to display to the user.
// @line_no: The line on which the error occured.
// 
// Version 4.4
// ----------------------------------------------------------
void DataTypingPhase::dataType_error(string err_msg, int line_no)
{
	cout << "DATA TYPE ERROR on line " << line_no << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

void DataTypingPhase::processDoubleOperator(BinaryOpNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}