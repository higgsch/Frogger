//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// ----------------------------------------------------------------
// This program represents a visitor for checking data types.
// -----------------------------------------------------------------
#include "fgrDataTypingPhase.h"
#include "..\SCFPhases\dataTypingPhase.h"
#include <string>
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This function starts the phase on the program root node.
// @n: The node representing the program.
//
// Version 2.3
// ----------------------------------------------------------
void FGRDataTypingPhase::visit(ProgramNode * n)
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
void FGRDataTypingPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	DataType * type = n->getDataType();
	Symbol sym = Symbol(id, DataType::DT_NOT_DEFINED, true);

	if (type == DataType::DT_NOT_DEFINED)
	{
		if (tables->syms->isDefined(&sym))
		{
			n->setDataType(tables->syms->symbolType(id));
			return;
		}

		if (setUnknownTypeNodesToDefault)
			tables->syms->add(new SymbolRecord(id, DataType::DT_DOUBLE, true));

		return;
	}
	
	if (!tables->syms->isDefined(&sym))
		tables->syms->add(new SymbolRecord(id, type, true));
	else
	{
		if (*(tables->syms->symbolType(id)) != *type)
			dataType_error("Variable " + id + " used as multiple type", n->getLineNo());
	}
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 2.3
// ----------------------------------------------------------
void FGRDataTypingPhase::visit(AssigningNode * n)
{
	n->visitAllChildren(this);
	unifyTreeDataType(n);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 5.1
// ----------------------------------------------------------
void FGRDataTypingPhase::visit(FunctionCallNode * n)
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
	if (!n->hasParentScope())
	{
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
			if (parentPhase->allTables->functs->getNumberOfMatches(funct) == 1)
			{
				n->setFunct(parentPhase->allTables->functs->getFirstMatch(funct));
				n->getPrimary()->setDataType(n->getFunct()->primary);
				n->visitAllChildren(this);
			}
		}
	}
	else
	{
		if (funct->primary->isNull() || n->getPrimary() == NULL || (n->getPrimary()->getDataType()->isDefined()))
		{ //parent of current scope or parent of primary scope
			FunctionTable * functs = getPrimaryParentScopeFunctions(n);
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
		{ //try to type primary
			if (parentPhase->allTables->functs->getNumberOfMatches(funct) == 1)
			{
				Routine * match = parentPhase->allTables->functs->getFirstMatch(funct);
				n->setFunct(match);
				n->getPrimary()->setDataType(getParentType(match->primary));
				n->visitAllChildren(this);
			}
		}
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 5.1
// ----------------------------------------------------------
void FGRDataTypingPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);
	unifyCommandCall(n);
	
	Routine * cmd = n->getCmd();

	if (!n->hasParentScope())
	{
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
			if (parentPhase->allTables->cmds->getNumberOfMatches(cmd) == 1)
			{
				n->setCmd(parentPhase->allTables->cmds->getFirstMatch(cmd));
				n->getPrimary()->setDataType(n->getCmd()->primary);
				n->visitAllChildren(this);
			}
		}
	}
	else
	{
		if (cmd->primary->isNull() || n->getPrimary() == NULL || (n->getPrimary()->getDataType()->isDefined()))
		{ //parent of current scope or parent of primary scope
			CommandTable * cmds = getPrimaryParentScopeCommands(n);
			if (!cmds->matchExists(cmd))
				dataType_error("Function call does not match signature: " + cmd->name, n->getLineNo());
			else if (cmds->getNumberOfMatches(cmd) == 1)
			{
				n->setCmd(cmds->getFirstMatch(cmd));
				checkAndSetNodeDataType(n, cmd->returnType);
				n->visitAllChildren(this);
			}
		}
		else
		{ //try to type primary
			if (parentPhase->allTables->functs->getNumberOfMatches(cmd) == 1)
			{
				Routine * match = parentPhase->allTables->functs->getFirstMatch(cmd);
				n->setCmd(match);
				n->getPrimary()->setDataType(getParentType(match->primary));
				n->visitAllChildren(this);
			}
		}
	}
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 5.0
// ----------------------------------------------------------
void FGRDataTypingPhase::visit(ArgListNode * n)
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
void FGRDataTypingPhase::visit(AddingNode * n)
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
void FGRDataTypingPhase::checkAndSetNodeDataType(AsciiNode * node, DataType* type)
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
void FGRDataTypingPhase::checkAndSetTreeDataType(TerminalNode * node, DataType* type)
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
void FGRDataTypingPhase::checkAndSetTreeDataType(UnaryNode * node, DataType * type)
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
void FGRDataTypingPhase::checkAndSetTreeDataType(BinaryNode * node, DataType* type)
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
void FGRDataTypingPhase::checkAndSetArgDataType(Routine * rout, int argNo, DataType * type, int lineNo)
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
void FGRDataTypingPhase::unifyTreeDataType(TerminalNode * node)
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
void FGRDataTypingPhase::unifyTreeDataType(UnaryNode * node)
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
void FGRDataTypingPhase::unifyTreeDataType(BinaryNode * node)
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
FunctionTable * FGRDataTypingPhase::getPrimaryScopeFunctions(FunctionCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	if (primary == NULL)
		return tables->functs;

	DataType * primaryType = primary->getDataType();
	if (primaryType == DataType::DT_NULL)
		return tables->functs;

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new FunctionTable();

	return parentPhase->getScopedTables(primaryType)->functs;
}

// ----------------------------------------------------------
// This function returns a safe command table based on the 
// scope of the given node's primary.
// 
// Version 5.0
// ----------------------------------------------------------
CommandTable * FGRDataTypingPhase::getPrimaryScopeCommands(CommandCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	if (primary == NULL)
		return tables->cmds;

	DataType * primaryType = primary->getDataType();
	if (primaryType == DataType::DT_NULL)
		return tables->cmds;

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new CommandTable();

	return parentPhase->getScopedTables(primaryType)->cmds;
}

// ----------------------------------------------------------
// This function returns a safe function table based on the 
// primary scope's parent scope. Handles current scope's parent 
// as well.
// 
// Version 5.1
// ----------------------------------------------------------
FunctionTable * FGRDataTypingPhase::getPrimaryParentScopeFunctions(FunctionCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	DataType * primaryType = NULL;

	if (primary == NULL) //Current scope's parent scoped functions
		primaryType = currentType;
	else
	{
		primaryType = primary->getDataType();
		if (primaryType == DataType::DT_NULL) //Current scope's parent scoped functions
			primaryType = currentType;
	}

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new FunctionTable();

	DataType * primaryParentType = parentPhase->getParentType(primaryType);

	return parentPhase->getScopedTables(primaryParentType)->functs;
}

// ----------------------------------------------------------
// This function returns a safe command table based on the 
// primary scope's parent scope. Handles current scope's parent 
// as well.
// 
// Version 5.1
// ----------------------------------------------------------
CommandTable * FGRDataTypingPhase::getPrimaryParentScopeCommands(CommandCallNode * n)
{
	AsciiNode * primary = n->getPrimary();
	DataType * primaryType = NULL;

	if (primary == NULL)
		primaryType = currentType;
	else
	{
		primaryType = primary->getDataType();
		if (primaryType == DataType::DT_NULL)
			primaryType = currentType;
	}

	if (primaryType == DataType::DT_NOT_DEFINED)
		return new CommandTable();

	DataType * primaryParentType = parentPhase->getParentType(primaryType);

	return parentPhase->getScopedTables(primaryParentType)->cmds;
}

// ----------------------------------------------------------
// This function ensures the ASTNodes under the given node
// are in line with the function record.
// @n: The FunctionCallNode to check.
// 
// Version 5.0
// ----------------------------------------------------------
void FGRDataTypingPhase::unifyFunctionCall(FunctionCallNode * n)
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
// Version 5.1
// ----------------------------------------------------------
void FGRDataTypingPhase::unifyCommandCall(CommandCallNode * n)
{
	Routine * cmd = n->getCmd();

	//ensure primary matches
	if (!n->hasParentScope())
	{
		if (!cmd->primary->isNull() && n->getPrimary() != NULL)
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
	}
	else
	{
		if (!cmd->primary->isNull() && n->getPrimary() != NULL)
		{
			AsciiNode * primaryNode = n->getPrimary();
			DataType * primaryNodeType = primaryNode->getDataType();
			//if (primaryNodeType->isDefined() && !cmd->primary->isDefined())
			//	cmd->primary = primaryNodeType;
			//else 
			if (cmd->primary->isDefined() && !primaryNodeType->isDefined())
				primaryNode->setDataType(getParentType(cmd->primary));
			else if (cmd->primary->isDefined() && primaryNodeType->isDefined() && 
					*(getParentType(cmd->primary)) != *(primaryNodeType))
				dataType_error("Mismatched Primary Type for Command Call: " + n->getLexeme(), n->getLineNo());
		}
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
// This function displays an error message to the user and 
// terminates the program.
// @err_msg: The message to display to the user.
// @line_no: The line on which the error occured.
// 
// Version 5.0
// ----------------------------------------------------------
void FGRDataTypingPhase::dataType_error(string err_msg, int line_no)
{
	cout << "DATA TYPE ERROR in UDF " << udfName << " on line " << line_no << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

void FGRDataTypingPhase::processDoubleOperator(BinaryOpNode * n)
{
	n->visitAllChildren(this);
	checkAndSetTreeDataType(n, DataType::DT_DOUBLE);
}

DataType * FGRDataTypingPhase::getParentType(DataType * dt)
{ 
	return parentPhase->getParentType(dt); 
}