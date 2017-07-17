// -----------------------------------------------------------------
// This is the header for the CodeGeneratingPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include <fstream>
#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
#include "..\DataStructures\Tables\tables.h"
#include "..\DataStructures\OutputText\outputText.h"
#include "..\Printing\printer.h"
#include "..\DataStructures\OutputLanguage\cppLanguage.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating output code
// that reflects the current AST.
//
// Version 4.2
// ----------------------------------------------------------
class CodeGenerationPhase : public Phase
{
private:
	Printer * p;
	CPPLanguage * lang;

	string currUDFName;

	bool isNested(BinaryOpNode * n) { return n->getParenNesting() > 0; }
	string leftText(BinaryOpNode * n) { return n->getLeftOperand()->outputText; }
	string rightText(BinaryOpNode * n) { return n->getRightOperand()->outputText; }

	bool validBuiltInFunctionName(string name);
	bool validBuiltInCommandName(string name);
public:
	CodeGenerationPhase() : p(new Printer()), lang(new CPPLanguage()), currUDFName("<META>") {}

	void open(string filename) { p->open(filename); }
	void close() { p->close(); }

	void printMetaCode(ProgramStruct * progStruct);
	void printUDFCode(FunctionAST * UDF, UDFRecord * rec);
	void printPEFCode(FunctionAST * PEF, UDFRecord * rec);

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n) { n->outputText = lang->getStringLiteralText(n->getLexeme()); }
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