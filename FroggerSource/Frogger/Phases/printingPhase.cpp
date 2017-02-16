//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
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
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(FunctionCallNode * n)
{
	print("FunctionCallNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printLine(concat("argListLength = ", n->getArgListLength()));
	
	printCmdData(n->getCmd(), "cmd");
	printFunctData(n->getFunct(), "funct");

	printNodeData(n->getPrimary(), "primary");
	printNodeData(n->getArgList(), "argList");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(CommandCallNode * n)
{
	print("CommandCallNode {\n");
	indentDepth++;

	printAsciiInfo(n);
	printDataTypeInfo(n);

	printLine(concat("argListLength = ", n->getArgListLength()));
	
	printCmdData(n->getCmd(), "cmd");

	printNodeData(n->getPrimary(), "primary");
	printNodeData(n->getArgList(), "argList");

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(ArgListNode * n)
{
	print("ArgListNode {\n");
	indentDepth++;

	printLine(concat("argNo = ", n->getArgNo()));

	printAsciiInfo(n);
	printDataTypeInfo(n);
	printLine("isListTyped = " + trueOrFalse(n->isListTyped()));

	printCmdData(n->getCmd(), "cmd");

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
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(AddingNode * n)
{
	print("AddingNode {\n");
	indentDepth++;

	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(SubingNode * n)
{
	print("SubingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(MulingNode * n)
{
	print("MulingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(DivingNode * n)
{
	print("DivingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(ModDivingNode * n)
{
	print("ModDivingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(IDivingNode * n)
{
	print("IDivingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(RootingNode * n)
{
	print("RootingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(ExpingNode * n)
{
	print("ExpingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

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
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(LTingNode * n) 
{
	print("LTingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(GTingNode * n) 
{
	print("GTingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(EQingNode * n) 
{
	print("EQingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(LTEingNode * n) 
{
	print("LTEingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::visit(GTEingNode * n) 
{
	print("GTEingNode {\n");
	indentDepth++;
	
	printBinaryOpNodeInfo(n);

	indentDepth--;
	printLine("}");
}

// ----------------------------------------------------------
// This function prints the data typing information of a node.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printDataTypeInfo(Node* n)
{
	if (printingDataTypeInfo)
	{
		printLine("isTreeTyped = " + trueOrFalse(n->isTreeTyped()));
		printLine("isTyped = " + trueOrFalse(n->isTyped()));
		printLine("dataType = " + dataType(n->getDataType()));
	}
}

// ----------------------------------------------------------
// This function prints ascii summation related information
// of a node.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printAsciiInfo(AsciiNode* n)
{
	if (printingAsciiInfo)
	{
		printLine("lexeme = " + n->getLexeme());
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
void PrintingPhase::printBinaryOpNodeInfo(BinaryOpNode* n)
{
	printAsciiInfo(n);
	printDataTypeInfo(n);
	
	printNodeData(n->getLeftOperand(), "leftOperand");
	printNodeData(n->getRightOperand(), "rightOperand");
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
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printCmdData(Command* c, string name)
{
	if (c == NULL)
		printLine(name + " = NULL");
	else
	{
		print(indent() + name + " = ");
		//TODO display data
		print("\n");
	}
}

// ----------------------------------------------------------
// This function safely prints a Function's data.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void PrintingPhase::printFunctData(Function* f, string name)
{
	if (f == NULL)
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
	
// ----------------------------------------------------------
// This function returns a string representation of the 
// DataType.
//
// Version 3.0
// ----------------------------------------------------------
string PrintingPhase::dataType(DataType dt)
{
	switch (dt)
	{
	case DT_NULL:
		return "NULL";
	case DT_DOUBLE:
		return "DOUBLE";
	case DT_STRING:
		return "STRING";
	case DT_NOT_DEFINED:
		return "NOT DEFINED";
	default:
		return "ERROR";
	}
}