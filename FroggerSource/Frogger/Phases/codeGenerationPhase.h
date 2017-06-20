// -----------------------------------------------------------------
// This is the header for the CodeGeneratingPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include <fstream>
#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
#include "..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating output code
// that reflects the current AST.
//
// Version 4.0
// ----------------------------------------------------------
class CodeGenerationPhase : public Phase
{
private:
	ofstream* out; // the output stream to print to
	int dblTempNo; // the current double temporary number in a line
	int strTempNo; // the current string temporary number in a line
	int indentDepth; // the number of tabs to insert

	bool needsRand; // flag for random()
	bool needsInFile; // flag for in_file object
	bool needsOutFile; // flag for out_file object

	void printForwardDeclarations(ProgramStruct * prog);
	void printFunctionPrototype(UDFRecord * rec);

	string typeString(DataType dt);
	string argsString(vector<argPair *> *);

public:
	CodeGenerationPhase();

	void open(string filename) { out->open(filename); }
	void close() { out->close(); }

	void printMetaCode(ProgramAST * progAST, ProgramStruct * progStruct);
	void printUDFCode(FunctionAST * UDF, UDFRecord * rec);
	void printPEFCode(FunctionAST * PEF, UDFRecord * rec);

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