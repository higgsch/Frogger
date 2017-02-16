// -----------------------------------------------------------------
// This is the header for the VarDecSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
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
	void visit(AssigningNode * n) { n->visitAllChildren(this); }
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n) { n->visitAllChildren(this); }
	void visit(ArgListNode * n) { n->visitAllChildren(this); }
	void visit(StringConstingNode * n){}
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n) { n->visitAllChildren(this); }
	void visit(SubingNode * n) { n->visitAllChildren(this); }
	void visit(MulingNode * n) { n->visitAllChildren(this); }
	void visit(DivingNode * n) { n->visitAllChildren(this); }
	void visit(ModDivingNode * n) { n->visitAllChildren(this); }
	void visit(IDivingNode * n) { n->visitAllChildren(this); }
	void visit(RootingNode * n) { n->visitAllChildren(this); }
	void visit(ExpingNode * n) { n->visitAllChildren(this); }
	void visit(NotingNode * n) { n->visitOperand(this); }
	void visit(LTingNode * n) { n->visitAllChildren(this); }
	void visit(GTingNode * n) { n->visitAllChildren(this); }
	void visit(EQingNode * n) { n->visitAllChildren(this); }
	void visit(LTEingNode * n) { n->visitAllChildren(this); }
	void visit(GTEingNode * n) { n->visitAllChildren(this); }

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