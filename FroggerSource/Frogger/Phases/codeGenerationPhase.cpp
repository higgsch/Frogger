//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function generates the pre-program code.
// @progStruct: The structure of the program
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printMetaCode(ProgramStruct * progStruct)
{
	p->printString(lang->getMetaCode(progStruct));
}

// ----------------------------------------------------------
// This function generates the PEF code.
// @PEF: The AST for the PEF
// @rec: The record of the PEF
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printPEFCode(FunctionAST * PEF, UDFRecord * rec)
{
	currUDFName = rec->UDFName;

	PEF->root->accept(this);
	string pefText = PEF->root->outputText;
	p->printString(lang->getPEFCode(rec, PEF->symbols, pefText));
}

// ----------------------------------------------------------
// This function generates the code for a given UDF.
// @UDF: The UDF's struct.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printUDFCode(FunctionAST * UDF, UDFRecord * rec) 
{
	currUDFName = rec->UDFName;

	UDF->root->accept(this);
	string udfText = UDF->root->outputText;
	p->printString(lang->getUDFCode(rec, UDF->symbols, udfText));
}

// ----------------------------------------------------------
// This function processes a user-defined routine.
// @n: The node representing the routine.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ProgramNode* n)
{
	n->visitAllChildren(this); 
	n->outputText = n->getFirstStmt()->outputText;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(JmpStmtNode * n)
{
	n->visitThisStmt(this);
	string thisStmtText = n->getStmt()->outputText;

	int stmtNo = n->getStmtNo();
	int jmpNo = n->getJump();
	bool isOwnLine = (!n->isNested());

	string currStmtText = lang->getJmpStmtText(currUDFName, stmtNo, jmpNo, isOwnLine, thisStmtText);

	n->visitNextStmt(this);
	ControlFlowNode * nextStmt = n->getNextStmt();
	string stmtTailText = (nextStmt == NULL) ? "" : nextStmt->outputText;

	n->outputText = currStmtText + stmtTailText;
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IfNode * n)
{
	n->visitBoolExp(this);
	n->visitTrueStmt(this);
	n->visitFalseStmt(this);
	string boolExpText = n->getBoolExp()->outputText;
	string trueStmtText = n->getTrueStmt()->outputText;
	string falseStmtText = n->getFalseStmt()->outputText;

	bool isOwnLine = (!n->isNested());
	int stmtNo = n->getStmtNo();

	string currStmtText = lang->getIfStmtText(currUDFName, stmtNo, isOwnLine,
										boolExpText, trueStmtText, falseStmtText);

	n->visitNextStmt(this);
	ControlFlowNode * nextStmt = n->getNextStmt();
	string stmtTailText = (nextStmt == NULL) ? "" : nextStmt->outputText;

	n->outputText = currStmtText + stmtTailText;
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IdRefNode * n)
{
	bool nested = n->getParenNesting() > 0;
	string id = n->getLexeme();

	n->outputText = lang->getIdentifierText(nested, id);
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AssigningNode * n)
{
	n->visitAssignee(this);
	n->visitAssignor(this);
	string assigneeText = n->getAssignee()->outputText;
	string assignorText = n->getAssignor()->outputText;

	n->outputText = lang->getAssignmentText(assigneeText, assignorText);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(FunctionCallNode * n)
{
	Function* funct = n->getFunct();

	string name = funct->name;
	bool isBuiltIn = !(funct->isUserDefined());

	if (isBuiltIn)
	{
		if (!validBuiltInFunctionName(name))
			semantic_error("Unrecognized function: " + name, n->getLineNo());
	}

	n->visitPrimary(this);
	AsciiNode* primary = n->getPrimary();
	n->visitArgList(this);
	AsciiNode* argList = n->getArgList();
	string primaryText = (primary == NULL) ? "" : primary->outputText;
	string argListText = (argList == NULL) ? "" : argList->outputText;

	n->outputText = lang->getFunctionCallText(isBuiltIn, primaryText, name, argListText);
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(CommandCallNode * n)
{
	Command* cmd = n->getCmd();

	string name = cmd->name;
	bool isBuiltIn = !(cmd->isUserDefined());

	if (isBuiltIn && !validBuiltInCommandName(name))
		semantic_error("Unrecognized command: " + name, n->getLineNo());

	n->visitPrimary(this);
	AsciiNode* primary = n->getPrimary();
	n->visitArgList(this);
	AsciiNode* argList = n->getArgList();
	string primaryText = (primary == NULL) ? "" : primary->outputText;
	string argListText = (argList == NULL) ? "" : argList->outputText;

	n->outputText = lang->getCommandCallText(isBuiltIn, primaryText, name, argListText);
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ArgListNode * n)
{
	n->visitThisArg(this);
	AsciiNode* thisArg = n->getThisArg();
	n->visitNextArg(this);
	AsciiNode* nextArg = n->getNextArg();

	string thisArgText = (thisArg == NULL) ? "" : thisArg->outputText;
	string nextArgText = (nextArg == NULL) ? "" : nextArg->outputText;

	n->outputText = lang->getArgumentListText(thisArgText, nextArgText);
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DoubleConstingNode * n)
{
	bool isNested = n->getParenNesting() > 0;
	string dbl = n->getLexeme();

	n->outputText = lang->getDoubleLiteralText(isNested, dbl);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(NotingNode * n) 
{
	n->visitOperand(this);
	string notText = n->getOperand()->outputText;

	n->outputText = lang->getNotOperationText(notText);
}

// ----------------------------------------------------------
// This function determines if the given name is a valid
// built-in function name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::validBuiltInFunctionName(string name)
{
	return name == lang->FUNCTNAME_TO_STRING ||
		   name == lang->FUNCTNAME_TO_ASCII ||
		   name == lang->FUNCTNAME_PARSE_DOUBLE ||
		   name == lang->FUNCTNAME_ASCII_AT ||
		   name == lang->FUNCTNAME_LENGTH ||
		   name == lang->FUNCTNAME_RETRIEVE_DOUBLE ||
		   name == lang->FUNCTNAME_RETRIEVE_STRING ||
		   name == lang->FUNCTNAME_RANDOM ||
		   name == lang->FUNCTNAME_READ ||
		   name == lang->FUNCTNAME_ELEMENT_AT ||
		   name == lang->FUNCTNAME_SIZE;
}

// ----------------------------------------------------------
// This function determines if the given name is a valid
// built-in command name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::validBuiltInCommandName(string name)
{
	return name == lang->CMDNAME_END_NULL ||
		   name == lang->CMDNAME_DISPLAY_STR ||
		   name == lang->CMDNAME_OPEN_INPUT ||
		   name == lang->CMDNAME_OPEN_OUTPUT ||
		   name == lang->CMDNAME_WRITE ||
		   name == lang->CMDNAME_CLOSE_INPUT ||
		   name == lang->CMDNAME_CLOSE_OUTPUT;
}

// ----------------------------------------------------------
// This function processes an operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visitBinaryOpNode(BinaryOpNode * n)
{
	n->visitAllChildren(this);
	bool isString = n->getDataType() == DT_STRING;
	n->outputText = lang->getBinaryOpText(isNested(n), isString, leftText(n), rightText(n), n->getOpType());
}