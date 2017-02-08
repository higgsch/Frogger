//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(JmpStmtNode * n)
{
	n->getStmt()->accept(this);
	
	bool isOwnLine = (!n->isNested());
	if (isOwnLine)
	{
		if (n->getNextStmt() != NULL)
			n->getNextStmt()->accept(this);
	}
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(IfNode * n)
{
	n->getBoolExp()->accept(this);
	n->getTrueStmt()->accept(this);
	n->getFalseStmt()->accept(this);

	bool isOwnLine = (!n->isNested());
	if (isOwnLine && n->getNextStmt() != NULL)
		n->getNextStmt()->accept(this);
}

// ----------------------------------------------------------
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RetrievalNode * n)
{
	checkAndSetDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DisplayingNode * n)
{
	AbstractNode *child = n->getLeftChild();
	child->accept(this);
}

// ----------------------------------------------------------
// This function processes a random statement.
// @n: The node representing the random statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RandomingNode * n)
{
	checkAndSetDataType(n, DT_DOUBLE);
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
		return;
	
	if (!symbols->symbolDefined(id))
		symbols->addSymbol(id,type);
	else
	{
		if (symbols->symbolType(id) != type)
			dataType_error("Variable " + id + " used as multiple type");
	}
}

// ----------------------------------------------------------
// This function processes a double assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AssigningDoubleNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a string assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AssigningStringNode * n)
{
	DataType type = DT_STRING;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(StringConstingNode * n)
{
	checkAndSetDataType(n, DT_STRING);
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DoubleConstingNode * n)
{
	checkAndSetDataType(n, DT_DOUBLE);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AddingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(SubingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(MulingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DivingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(ModDivingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(IDivingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(RootingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(ExpingNode * n)
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(StringConcatingNode * n)
{
	DataType type = DT_STRING;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(DoubleConcatingNode * n)
{
	checkAndSetDataType(n, DT_STRING);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, DT_STRING);
	checkAndSetDataType(right, DT_DOUBLE);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(AsciiConcatingNode * n)
{
	checkAndSetDataType(n, DT_STRING);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, DT_STRING);
	checkAndSetDataType(right, DT_DOUBLE);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(NotingNode * n) 
{
	n->getLeftChild()->accept(this);
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(LTingNode * n) 
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(GTingNode * n) 
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::visit(EQingNode * n) 
{
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
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
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
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
	DataType type = DT_DOUBLE;
	checkAndSetDataType(n, type);

	AbstractNode *left = n->getLeftChild(), *right = n->getRightChild();
	checkAndSetDataType(left, type);
	checkAndSetDataType(right, type);
	left->accept(this);
	right->accept(this);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node. It throws
// a dataType error if there is a dataType conflict.
// 
// Version 2.3
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetDataType(AbstractNode * node, DataType type)
{
	if (node->getDataType() == type || type == DT_NOT_DEFINED)
		return;

	if (node->getDataType() == DT_NOT_DEFINED)
		node->setDataType(type);
	else
		dataType_error("Data Type Conflict");
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