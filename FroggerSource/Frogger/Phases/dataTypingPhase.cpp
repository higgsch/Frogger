//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
#include <string>
using namespace std;

extern bool quietMode;

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
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	DataType * type = n->getDataType();
	Symbol sym = Symbol(id, DataType::DT_NOT_DEFINED, true);

	if (type == DataType::DT_NOT_DEFINED)
	{
		if (symbols->isDefined(&sym))
		{
			n->setDataType(symbols->symbolType(id));
			return;
		}

		if (setUnknownTypeNodesToDefault)
			symbols->add(new SymbolRecord(id, DataType::DT_DOUBLE, true));

		return;
	}
	
	if (!symbols->isDefined(&sym))
		symbols->add(new SymbolRecord(id, type, true));
	else
	{
		if (*(symbols->symbolType(id)) != *type)
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
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::visit(FunctionCallNode * n)
{
	DataType* oldNodeType = n->getDataType();

	if (oldNodeType != DataType::DT_NOT_DEFINED)
		checkAndSetNodeDataType(n, oldNodeType);

	DataType * oldFunctType = n->getFunct()->returnType;
	if (oldFunctType != DataType::DT_NOT_DEFINED)
		checkAndSetNodeDataType(n, oldFunctType);
	else if (oldNodeType != DataType::DT_NOT_DEFINED)
		n->getFunct()->returnType = oldNodeType;

	n->visitAllChildren(this);
	unifyFunctionCall(n);

	Routine * funct = n->getFunct();
	if (funct->primary->isNull() || (n->getPrimary()->getDataType()->isDefined()))
	{
		FunctionTable * functs = getPrimaryScopeFunctions(n);
		if (!functs->matchExists(funct))
			dataType_error("Function call does not match signature: " + funct->name, n->getLineNo());
		else if (functs->getNumberOfMatches(funct) == 1)
		{
			n->setFunct(functs->getFirstMatch(funct));
			checkAndSetNodeDataType(n, funct->returnType);
			n->visitAllChildren(this);
		}
	}
	else
	{
		//try to type primary
		if (allFunctions->getNumberOfMatches(funct) == 1)
		{
			n->setFunct(allFunctions->getFirstMatch(funct));
			n->getPrimary()->setDataType(n->getFunct()->primary);
			n->visitAllChildren(this);
		}
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);
	unifyCommandCall(n);
	
	Routine * cmd = n->getCmd();

	if (cmd->primary->isNull() || (n->getPrimary()->getDataType()->isDefined()))
	{
		CommandTable * cmds = getPrimaryScopeCommands(n);
		if (!cmds->matchExists(cmd))
			dataType_error("Command call does not match signature: " + cmd->name, n->getLineNo());
		else if (cmds->getNumberOfMatches(cmd) == 1)
		{
			n->setCmd(cmds->getFirstMatch(cmd));
			n->visitAllChildren(this);
		}
	}
	else
	{
		//Try to type primary
		if (allCommands->getNumberOfMatches(cmd) == 1)
		{
			n->setCmd(allCommands->getFirstMatch(cmd));
			n->getPrimary()->setDataType(n->getCmd()->primary);
			n->visitAllChildren(this);
		}
	}
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::visit(ArgListNode * n)
{
	int argNo = n->getArgNo();
	DataType* oldArgType = n->getRoutine()->getArg(argNo)->type;
	if (oldArgType != DataType::DT_NOT_DEFINED)
		checkAndSetNodeDataType(n->getThisArg(),oldArgType);

	n->visitAllChildren(this);
	
	DataType* argType = n->getThisArg()->getDataType();
	if (argType != DataType::DT_NOT_DEFINED)
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
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetNodeDataType(AsciiNode * node, DataType* type)
{
	if (*(node->getDataType()) == *type || type == DataType::DT_NOT_DEFINED)
		return;

	if (node->getDataType() == DataType::DT_NOT_DEFINED)
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
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(TerminalNode * node, DataType* type)
{
	if (type == DataType::DT_NOT_DEFINED)
		return;

	DataType* thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children. It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(UnaryNode * node, DataType * type)
{
	if (type == DataType::DT_NOT_DEFINED)
		return;

	DataType* thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);

	AsciiNode *child = node->getChild();
	if (child != NULL)
	{
		DataType* leftOldType = child->getDataType();
		checkAndSetNodeDataType(child, type);
		if (leftOldType == DataType::DT_NOT_DEFINED)
			child->accept(this);
	}

	if (thisOldType == DataType::DT_NOT_DEFINED)
		node->accept(this);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children. It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetTreeDataType(BinaryNode * node, DataType* type)
{
	if (type == DataType::DT_NOT_DEFINED)
		return;

	DataType* thisOldType = node->getDataType();
	checkAndSetNodeDataType(node, type);

	AsciiNode *left = node->getLeftChild();
	if (left != NULL)
	{
		DataType* leftOldType = left->getDataType();
		checkAndSetNodeDataType(left, type);
		if (leftOldType == DataType::DT_NOT_DEFINED)
			left->accept(this);
	}

	AsciiNode *right = node->getRightChild();
	if (right != NULL)
	{
		DataType* rightOldType = right->getDataType();
		checkAndSetNodeDataType(right, type);
		if (rightOldType == DataType::DT_NOT_DEFINED)
			right->accept(this);
	}

	if (thisOldType == DataType::DT_NOT_DEFINED)
		node->accept(this);
}

// ----------------------------------------------------------
// This function safely sets the dataType of an argument. 
// It throws a dataType error if there is a dataType 
// conflict.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::checkAndSetArgDataType(Routine * rout, int argNo, DataType * type, int lineNo)
{
	DataType* oldType = rout->getArg(argNo)->type;
	if (*oldType == *type || type == DataType::DT_NOT_DEFINED)
		return;

	if (oldType == DataType::DT_NOT_DEFINED)
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
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(TerminalNode * node)
{
	DataType* type = node->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}
	
	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DataType::DT_DOUBLE);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(UnaryNode * node)
{
	DataType* type = node->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *child = node->getChild();
	type = child->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DataType::DT_DOUBLE);
}

// ----------------------------------------------------------
// This function safely sets the dataType of a node and all 
// children based on the node and children's dataType. It 
// throws a dataType error if there is a dataType conflict.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::unifyTreeDataType(BinaryNode * node)
{
	DataType * type = node->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *left = node->getLeftChild();
	type = left->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	AsciiNode *right = node->getRightChild();
	type = right->getDataType();
	if (type != DataType::DT_NOT_DEFINED)
	{
		checkAndSetTreeDataType(node, type);
		return;
	}

	//All nodes are DT_NOT_DEFINED
	if (setUnknownTypeNodesToDefault)
		checkAndSetTreeDataType(node, DataType::DT_DOUBLE);
}

// ----------------------------------------------------------
// This function returns a safe function table based on the 
// scope of the given node's primary.
// 
// Version 5.0
// ----------------------------------------------------------
FunctionTable * DataTypingPhase::getPrimaryScopeFunctions(FunctionCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	if (primary == NULL)
		return functions;

	DataType * primaryType = primary->getDataType();
	if (primaryType == DataType::DT_NULL)
		return functions;

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new FunctionTable();

	return progStruct->getObject(primary->getDataType())->scopedTables->functs;
}

// ----------------------------------------------------------
// This function returns a safe command table based on the 
// scope of the given node's primary.
// 
// Version 5.0
// ----------------------------------------------------------
CommandTable * DataTypingPhase::getPrimaryScopeCommands(CommandCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	if (primary == NULL)
		return commands;

	DataType * primaryType = primary->getDataType();
	if (primaryType == DataType::DT_NULL)
		return commands;

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new CommandTable();

	return progStruct->getObject(primary->getDataType())->scopedTables->cmds;
}

// ----------------------------------------------------------
// This function ensures the ASTNodes under the given node
// are in line with the function record.
// @n: The FunctionCallNode to check.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::unifyFunctionCall(FunctionCallNode * n)
{
	Routine * funct = n->getFunct();

	//ensure args match
	int argCount = funct->getNumArgs();
	ArgListNode * currArgListNode = (ArgListNode* )n->getArgList();
	for (int argIndex = 0; argIndex < argCount; argIndex++)
	{
		if (currArgListNode == NULL)
			dataType_error("Mismatched Argument List Length for Command Call: " + n->getLexeme(), n->getLineNo());

		DataType * currFUNCTArgType = funct->getArg(argIndex)->type;
		DataType * currArgListNodeType = currArgListNode->getDataType();

		if (currArgListNodeType->isDefined() && !currFUNCTArgType->isDefined())
			funct->getArg(argIndex)->type = currArgListNodeType;
		else if (currFUNCTArgType->isDefined() && !currArgListNodeType->isDefined())
			currArgListNode->setDataType(currFUNCTArgType);
		else if (currFUNCTArgType->isDefined() && currArgListNodeType->isDefined()
					&& *currFUNCTArgType != *currArgListNodeType)
			dataType_error("Mismatched Argument Type for Command Call: " + n->getLexeme() + ", Argument No: " + to_string(argIndex), n->getLineNo());

		currArgListNode = (ArgListNode*) currArgListNode->getNextArg();
	}

	//ensure no more args in AST
	if (currArgListNode != NULL)
		dataType_error("Mismatched Argument List Length for Command Call: " + n->getLexeme(), n->getLineNo());
}

// ----------------------------------------------------------
// This function ensures the ASTNodes under the given node
// are in line with the command record.
// @n: The CommandCallNode to check.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::unifyCommandCall(CommandCallNode * n)
{
	Routine * cmd = n->getCmd();

	//ensure primary matches
	if (!cmd->primary->isNull())
	{
		AsciiNode * primaryNode = n->getPrimary();
		DataType * primaryNodeType = primaryNode->getDataType();
		if (primaryNodeType->isDefined() && !cmd->primary->isDefined())
			cmd->primary = primaryNodeType;
		else if (cmd->primary->isDefined() && !primaryNodeType->isDefined())
			primaryNode->setDataType(cmd->primary);
		else if (cmd->primary->isDefined() && primaryNodeType->isDefined() && *(cmd->primary) != *(primaryNodeType))
			dataType_error("Mismatched Primary Type for Command Call: " + n->getLexeme(), n->getLineNo());
	}

	//ensure args match
	int argCount = cmd->getNumArgs();
	ArgListNode * currArgListNode = (ArgListNode* )n->getArgList();
	for (int argIndex = 0; argIndex < argCount; argIndex++)
	{
		if (currArgListNode == NULL)
			dataType_error("Mismatched Argument List Length for Command Call: " + n->getLexeme(), n->getLineNo());

		DataType * currCMDArgType = cmd->getArg(argIndex)->type;
		DataType * currArgListNodeType = currArgListNode->getDataType();

		if (currArgListNodeType->isDefined() && !currCMDArgType->isDefined())
			cmd->getArg(argIndex)->type = currArgListNodeType;
		else if (currCMDArgType->isDefined() && !currArgListNodeType->isDefined())
			currArgListNode->setDataType(currCMDArgType);
		else if (currCMDArgType->isDefined() && currArgListNodeType->isDefined()
					&& *currCMDArgType != *currArgListNodeType)
			dataType_error("Mismatched Argument Type for Command Call: " + n->getLexeme() + ", Argument No: " + to_string(argIndex), n->getLineNo());

		currArgListNode = (ArgListNode*) currArgListNode->getNextArg();
	}

	//ensure no more args in AST
	if (currArgListNode != NULL)
		dataType_error("Mismatched Argument List Length for Command Call: " + n->getLexeme(), n->getLineNo());
}

// ----------------------------------------------------------
// This function merges all scoped tables within the given
// ObjectStruct into cumulative tables.
// @obj: The object to accumulate.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::populateAllTables(ObjectStruct* obj)
{
	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		populateAllTables(obj->getOF(objIndex));
	}

	allCommands->merge(obj->scopedTables->cmds);
	allFunctions->merge(obj->scopedTables->functs);
	allSymbols->merge(obj->scopedTables->syms);
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @err_msg: The message to display to the user.
// @line_no: The line on which the error occured.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::dataType_error(string err_msg, int line_no)
{
	cout << "DATA TYPE ERROR in UDF " << udfName << " on line " << line_no << ": " << err_msg << endl;
	
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
	checkAndSetTreeDataType(n, DataType::DT_DOUBLE);
}