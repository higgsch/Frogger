// -----------------------------------------------------------------
// This is the header for the PrintingPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "phase.h"
#include "..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for displaying the AST to
// an output stream.
//
// Version 3.0
// ----------------------------------------------------------
class PrintingPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	int indentDepth; // the number of indents to insert

	bool printingDataTypeInfo;
	bool printingAsciiInfo;
	bool printingLineNumberInfo;

	void printDataTypeInfo(Node* n);
	void printAsciiInfo(AsciiNode* n);
	void printBinaryOpNodeInfo(BinaryOpNode* n);

	void printNodeData(Node* n, string name);
	void printCmdData(Command* c, string name);
	void printFunctData(Function* f, string name);

	void printLine(string line) { *out << indent() << line << endl; }
	void print(string toPrint) { *out << toPrint; }
	
	string indent();
	string trueOrFalse(bool b) { return b ? "true" : "false"; }
	string dataType(DataType dt);
	string concat(string s, int i) { return s + to_string(i); }

public:
	PrintingPhase(ostream* i_out);

	void includeDataTypingInfo() { printingDataTypeInfo = true; }
	void includeAsciiInfo() { printingAsciiInfo = true; }
	void includeLineNumberInfo() { printingLineNumberInfo = true; }

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n);
	void visit(SubingNode * n);
	void visit(MulingNode * n);
	void visit(DivingNode * n);
	void visit(ModDivingNode * n);
	void visit(IDivingNode * n);
	void visit(RootingNode * n);
	void visit(ExpingNode * n);
	void visit(NotingNode * n);
	void visit(LTingNode * n);
	void visit(GTingNode * n);
	void visit(EQingNode * n);
	void visit(LTEingNode * n);
	void visit(GTEingNode * n);
};