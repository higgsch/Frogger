//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program represents a visitor for displaying the AST to
// an output stream.
// -----------------------------------------------------------------
#include "printingPhase.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
PrintingPhase::PrintingPhase(ostream* i_out)
{
	out = i_out;
	indentDepth = 0;
	printingDataTypeInfo = false;
	printingAsciiInfo = false;
	printingLineNumberInfo = false;
}

// ----------------------------------------------------------
// This function initiates the phase over the AST.
// @n: The node representing the program.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(ProgramNode * n)
{
	cout << "Printing AST" << endl;

	printLine("ProgramNode {");
	indentDepth++;

	if (printingLineNumberInfo)
		printLine(concat("lineCount = ", n->getLineCount()));

	printDataTypeInfo(n);

	printNodeData(n->getFirstStmt(), "firstStmt");

	indentDepth--;
	printLine("}");

	printLine(concat("indentDepth = ", indentDepth));
	printLine("");
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(JmpStmtNode * n)
{
	print("JmpStmtNode {\n");
	indentDepth++;

	if (printingLineNumberInfo)
	{
		printLine(concat("stmtNo = ", n->getStmtNo()));
		printLine(concat("jumpTo = ", n->getJump()));
	}

	printLine("isNested = " + trueOrFalse(n->isNested()));

	printDataTypeInfo(n);
	
	printNodeData(n->getStmt(), "stmt");
	printNodeData(n->getNextStmt(), "nextStmt");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(IfNode * n)
{
	print("IfStmtNode {\n");
	indentDepth++;

	if (printingLineNumberInfo)
		printLine(concat("stmtNo = ", n->getStmtNo()));

	printLine("isNested = " + trueOrFalse(n->isNested()));

	printDataTypeInfo(n);

	printNodeData(n->getBoolExp(), "boolExp");
	printNodeData(n->getTrueStmt(), "trueStmt");
	printNodeData(n->getFalseStmt(), "falseStmt");

	printNodeData(n->getNextStmt(), "nextStmt");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(IdRefNode * n)
{
	print("IdRefNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(AssigningNode * n)
{
	print("AssigningNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printNodeData(n->getAssignee(), "assignee");
	printNodeData(n->getAssignor(), "assignor");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void PrintingPhase::visit(FunctionCallNode * n)
{
	print("FunctionCallNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printLine(concat("argListLength = ", n->getArgListLength()));
	
	printRoutineData(n->getFunct(), "rout");

	printNodeData(n->getPrimary(), "primary");
	printNodeData(n->getArgList(), "argList");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void PrintingPhase::visit(CommandCallNode * n)
{
	print("CommandCallNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printLine(concat("argListLength = ", n->getArgListLength()));
	
	printRoutineData(n->getCmd(), "rout");

	printNodeData(n->getPrimary(), "primary");
	printNodeData(n->getArgList(), "argList");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void PrintingPhase::visit(ArgListNode * n)
{
	print("ArgListNode {\n");
	indentDepth++;

	printLine(concat("argNo = ", n->getArgNo()));

	printAsciiInfo(n);
	printDataTypeInfo(n);
	printLine("isListTyped = " + trueOrFalse(n->isListTyped()));

	printRoutineData(n->getRoutine(), "rout");

	printLine("hasNextArg = " + trueOrFalse(n->hasNextArg()));
	
	printNodeData(n->getThisArg(), "arg");
	printNodeData(n->getNextArg(), "nextArg");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(StringConstingNode * n)
{
	print("StringConstingNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(DoubleConstingNode * n)
{
	print("DoubleConstingNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(NotingNode * n) 
{
	print("NotingNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printNodeData(n->getOperand(), "operand");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function prints the data typing information of a node.
// @n: The node representing the operation.
//
// Version 5.0
// ----------------------------------------------------------
void PrintingPhase::printDataTypeInfo(Node* n)
{
	if (printingDataTypeInfo)
	{
		printLine("isTreeTyped = " + trueOrFalse(n->isTreeTyped()));
		printLine("isTyped = " + trueOrFalse(n->isTyped()));
		printLine("dataType = " + n->getDataType()->typeString);
	}
}

// ----------------------------------------------------------
// This function prints ascii summation related information
// of a node.
// @n: The node representing the operation.
//
// Version 3.1
// ----------------------------------------------------------
void PrintingPhase::printAsciiInfo(AsciiNode* n)
{
	if (printingAsciiInfo)
	{
		printLine("lexeme = " + n->getLexeme());
		printLine("lineNo = " + to_string(n->getLineNo()));
		printLine(concat("parenCount = ", n->getParenNesting()));
		printLine(concat("asciiSum = ", n->getAscii()));
	}
}

// ----------------------------------------------------------
// This function prints the information common to 
// BinaryOpNodes.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printBinaryOpNodeInfo(BinaryOpNode* n, string name)
{
	print(name + " {\n");
	indentDepth++;
	
	printAsciiInfo(n);
	printDataTypeInfo(n);
	
	printNodeData(n->getLeftOperand(), "leftOperand");
	printNodeData(n->getRightOperand(), "rightOperand");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function safely prints a Node's data.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printNodeData(Node* n, string name)
{
	if (n == NULL)
		printLine(name + " = NULL");
	else
	{
		print(indent() + name + " = ");
		n->accept(this);
	}
}

// ----------------------------------------------------------
// This function safely prints a Command's data.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void PrintingPhase::printRoutineData(Routine* r, string name)
{
	if (r == NULL)
		printLine(name + " = NULL");
	else
	{
		print(indent() + name + " = ");
		//TODO display data
		print("\n");
	}
}
	
// ----------------------------------------------------------
// This function returns a string containing the current space
// indentation.
//
// Version 3.0
// ----------------------------------------------------------
string PrintingPhase::indent()
{
	string result = "";
	for (int i = 0; i < indentDepth; i++)
		result = result + " ";

	return result;
}