//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
DataTypingPhase::DataTypingPhase() 
{ 
	symbols = new SymbolTable(); 
	functions = new FunctionTable();
	commands = new CommandTable();
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
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(JmpStmtNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(IfNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	DataType type = n->getDataType();

	if (type == DT_NOT_DEFINED)
	{
		if (symbols->symbolDefined(id))
		{
			n->setDataType(symbols->symbolType(id));
			return;
		}

		if (setUnknownTypeNodesToDefault)
			symbols->addSymbol(id, DT_DOUBLE);

		return;
	}
	
	if (!symbols->symbolDefined(id))
		symbols->addSymbol(id,type);
	else
	{
		if (symbols->symbolType(id) != type)
			dataType_error("Variable " + id + " used as multiple type");
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
// Version 2.4
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
		dataType_error("Function call does not match signature: " + funct->name);
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
// Version 2.5
// ----------------------------------------------------------
void DataTypingPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Command * cmd = n->getCmd();
	if (!commands->matchExists(cmd))
		dataType_error("Command call does not match signature: " + cmd->name);
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
// Version 2.5
// ----------------------------------------------------------
void DataTypingPhase::visit(ArgListNode * n)
{
	int argNo = n->getArgNo();
	DataType oldArgType = n->getCmd()->getDataTypeOfArgNumber(argNo);
	if (oldArgType != DT_NOT_DEFINED)
		checkAndSetNodeDataType(n->getThisArg(),oldArgType);

	n->visitAllChildren(this);
	
	DataType argType = n->getThisArg()->getDataType();
	if (argType != DT_NOT_DEFINED)
	{
		checkAndSetNodeDataType(n, argType);
		Command * cmd = n->getCmd();
		int argNo = n->getArgNo();
		checkAndSetArgDataType(cmd,argNo,argType);
	}
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(StringConstingNode * n)
{
	checkAndSetNodeDataType(n, DT_STRING);
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DoubleConstingNode * n)
{
	checkAndSetNodeDataType(n, DT_DOUBLE);
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
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(SubingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(MulingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DivingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(ModDivingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(IDivingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RootingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(ExpingNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void DataTypingPhase::visit(NotingNode * n) 
{
	n->visitOperand(this);
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(LTingNode * n) 
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(GTingNode * n) 
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(EQingNode * n) 
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(LTEingNode * n) 
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(GTEingNode * n) 
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node. It throws
// a dataType error if there is a dataType conflict.
// 
// Version 2.3
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
		dataType_error("Data Type Conflict");
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
// Version 2.4
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetArgDataType(Command * cmd, int argNo, DataType type)
{
	DataType oldType = cmd->getDataTypeOfArgNumber(argNo);
	if (oldType == type || type == DT_NOT_DEFINED)
		return;

	if (oldType == DT_NOT_DEFINED)
	{
		cmd->setDataTypeOfArgNumber(argNo, type);
		return;
	}
	else
		dataType_error("Data Type Conflict");
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
// 
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::dataType_error(string err_msg)
{
	cout << "DATA TYPE ERROR: " << err_msg << endl;
	cout << "Press Enter to Exit" << endl;

	getchar();
	exit(0);
}