// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phases.h"
#include "..\Parsing\nodes.h"
#include "symbolTable.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for checking data types
//
// Version 2.3
// ----------------------------------------------------------
class DataTypingPhase : public Phase
{
private:
	SymbolTable * symbols;
	bool changeMadeThisRound;
	bool setUnknownTypeNodesToDefault;

	void checkAndSetNodeDataType(AbstractNode * node, DataType type);
	void checkAndSetTreeDataType(AbstractNode * node, DataType type);
	void unifyTreeDataType(AbstractNode * node);
	void dataType_error(string);

public:
	DataTypingPhase() 
	{ 
		symbols = new SymbolTable(); 
		changeMadeThisRound = false; 
		setUnknownTypeNodesToDefault = false;
	}

	SymbolTable * getSymbolTable();

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n);
	void visit(IfNode * n);
	void visit(RetrievalNode * n);
	void visit(DisplayingNode * n);
	void visit(RandomingNode * n);
	void visit(EndingNode * n);
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
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