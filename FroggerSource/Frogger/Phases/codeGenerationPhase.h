// -----------------------------------------------------------------
// This is the header for the CodeGeneratingPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include <fstream>
#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
#include "..\DataStructures\Tables\tables.h"
#include "SubPhases\supportReqsSubPhase.h"
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
	ofstream* out; // the output stream to print to
	int indentDepth; // the number of tabs to insert
	string currUDFName;

	void emitUsingStatement();
	void importIOStream();
	void importMath();
	void importStdLib();
	void importTime();
	void importFStream();

	void emitSupportCode();
	void emitArgVector();
	void emitFileStreams();
	void emitEmptyString();
	void emitRoundFunction();
	void emitRtFunction();

	void printBuiltInFunctions();
	void printBuiltInCommands();
	void printForwardDeclarations(ProgramStruct * prog);
	void printMainFunction(string PEFName);

	void printIndent() { *out << indent(); }
	void printString(string s) { *out << s; }
	void printEmptyLine() { *out << endl; }
	void printLine(string s) { *out << indent() << s << endl; }
	void printOpenBraceLine() { printLine("{"); indentDepth++; }
	void printCloseBraceLine() { printLine("}"); indentDepth--; }

	string getFunctionPrototype(UDFRecord * rec);
	string getLabelText(int labelIndex);
	string typeString(DataType dt);
	string argsString(vector<argPair *> *);

	void processBinaryOpNode(BinaryOpNode * n, string pretext, string midtext, string posttext);

	bool validBuiltInFunctionName(string name);
	bool validBuiltInCommandName(string name);
public:
	CodeGenerationPhase();

	void open(string filename) { out->open(filename); }
	void close() { out->close(); }

	void printMetaCode(ProgramAST * progAST, ProgramStruct * progStruct);
	void printUDFCode(FunctionAST * UDF, UDFRecord * rec);
	void printPEFCode(FunctionAST * PEF, UDFRecord * rec) { printUDFCode(PEF, rec); }

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n) { *out << n->getLexeme(); }
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n);
	void visit(SubingNode * n) { processBinaryOpNode(n, "", " - ", ""); }
	void visit(MulingNode * n) { processBinaryOpNode(n, "", " * ", ""); }
	void visit(DivingNode * n) { processBinaryOpNode(n, "", " / ", ""); }
	void visit(ModDivingNode * n) { processBinaryOpNode(n, "fmod( ", ", ", ")"); }
	void visit(IDivingNode * n) { processBinaryOpNode(n, "((int)round(",")) / ((int)round(","))"); }
	void visit(RootingNode * n) { processBinaryOpNode(n, "rt(", ", ", ")"); }
	void visit(ExpingNode * n) { processBinaryOpNode(n, "pow(", ", ", ")"); }
	void visit(NotingNode * n);
	void visit(LTingNode * n) { processBinaryOpNode(n, "", " < ", ""); }
	void visit(GTingNode * n) { processBinaryOpNode(n, "", " > ", ""); }
	void visit(EQingNode * n) { processBinaryOpNode(n, "", " == ", ""); }
	void visit(LTEingNode * n) { processBinaryOpNode(n, "", " <= ", ""); }
	void visit(GTEingNode * n) { processBinaryOpNode(n, "", " >= ", ""); }
	
	// ----------------------------------------------------------
	// This function returns a string containing the current tab
	// indentation.
	//
	// Version 3.0
	// ----------------------------------------------------------
	string indent()
	{
		string result = "";
		for (int i = 0; i < indentDepth; i++)
			result = result + "\t";

		return result;
	}
};