// -----------------------------------------------------------------
// This is the header for the PrintingPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for displaying the AST to
// an output stream.
//
// Version 3.1
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
	void printBinaryOpNodeInfo(BinaryOpNode* n, string name);

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
	void visit(AddingNode * n) { printBinaryOpNodeInfo(n, "AddingNode"); }
	void visit(SubingNode * n) { printBinaryOpNodeInfo(n, "SubingNode"); }
	void visit(MulingNode * n) { printBinaryOpNodeInfo(n, "MulingNode"); }
	void visit(DivingNode * n) { printBinaryOpNodeInfo(n, "DivingNode"); }
	void visit(ModDivingNode * n) { printBinaryOpNodeInfo(n, "ModDivingNode"); }
	void visit(IDivingNode * n) { printBinaryOpNodeInfo(n, "IDivingNode"); }
	void visit(RootingNode * n) { printBinaryOpNodeInfo(n, "RootingNode"); }
	void visit(ExpingNode * n) { printBinaryOpNodeInfo(n, "ExpingNode"); }
	void visit(NotingNode * n);
	void visit(LTingNode * n) { printBinaryOpNodeInfo(n, "LTingNode"); }
	void visit(GTingNode * n) { printBinaryOpNodeInfo(n, "GTingNode"); }
	void visit(EQingNode * n) { printBinaryOpNodeInfo(n, "EQingNode"); }
	void visit(LTEingNode * n) { printBinaryOpNodeInfo(n, "LTEingNode"); }
	void visit(GTEingNode * n) { printBinaryOpNodeInfo(n, "GTEingNode"); }
};