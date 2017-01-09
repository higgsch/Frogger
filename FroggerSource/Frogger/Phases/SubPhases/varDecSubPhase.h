// -----------------------------------------------------------------
// This is the header for the VarDecSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include "..\phases.h"
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
// Version 2.0
// ----------------------------------------------------------
class VarDecSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	VarList* list; // the "symbol table"
	int currStmtTempCount; // a counter for the temporaries needed for the current line
	int lineTempMax; // a storage for the maximum temporaries needed in any given line

	void addToList(string);
	bool isInList(string);

public:
	VarDecSubPhase(ostream*);

	void visit(StmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(EndingNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(StringConstingNode * n);
	void visit(DoubleConstingNode * n);
	void visit(AddingNode * n);
	void visit(SubingNode * n);
	void visit(MulingNode * n);
	void visit(DivingNode * n);
	void visit(NotingNode * n);
	void visit(LTingNode * n);
	void visit(GTingNode * n);
	void visit(EQingNode * n);
	void visit(LTEingNode * n);
	void visit(GTEingNode * n);

	void addTemporaries();
};