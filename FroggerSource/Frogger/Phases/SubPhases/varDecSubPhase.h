// -----------------------------------------------------------------
// This is the header for the VarDecSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phase.h"
#include "..\..\Parsing\nodes.h"
using namespace std;

// A structure for a list of variables
// A simple dynamic "symbol table"
struct VarList
{
	string id; // the variable's lexeme
	VarList* tail; // the remaining variables
};

// ----------------------------------------------------------
// This class represents a visitor for generating variable
// declarations as a subphase of the CodeGenerationPhase.
//
// Version 3.0
// ----------------------------------------------------------
class VarDecSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	VarList* list; // the "symbol table"
	int currStmtDblTempCount; // a counter for the double temporaries needed for the current line
	int currStmtStrTempCount; // a counter for the string temporaries needed for the current line
	int lineDblTempMax; // a storage for the maximum double temporaries needed in any given line
	int lineStrTempMax; // a storage for the maximum string temporaries needed in any given line
	int indentDepth; // the number of tabs to insert

	void addToList(string);
	bool isInList(string);

public:
	VarDecSubPhase(ostream*, int tabCount);

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n){}
	void visit(DoubleConstingNode * n){}
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

	void addTemporaries();

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