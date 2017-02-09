//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
using namespace std;

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
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RetrievalNode * n)
{
	checkAndSetNodeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DisplayingNode * n)
{
	n->visitLeftChild(this);
}

// ----------------------------------------------------------
// This function processes a random statement.
// @n: The node representing the random statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RandomingNode * n)
{
	checkAndSetNodeDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes an end statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(EndingNode * n)
{
	// No op
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

	if (type = DT_NOT_DEFINED)
	{
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
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(NotingNode * n) 
{
	n->visitLeftChild(this);
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
void DataTypingPhase::checkAndSetNodeDataType(AbstractNode * node, DataType type)
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
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(AbstractNode * node, DataType type)
{
	if (type == DT_NOT_DEFINED)
		return;

	checkAndSetNodeDataType(node, type);

	AbstractNode *left = node->getLeftChild();
	checkAndSetNodeDataType(left, type);

	AbstractNode *right = node->getRightChild();
	checkAndSetNodeDataType(right, type);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(AbstractNode * node)
{
	DataType type = node->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AbstractNode *left = node->getLeftChild();
	type = left->getDataType();
	if (type != DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AbstractNode *right = node->getRightChild();
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