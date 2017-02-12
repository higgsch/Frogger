// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phase.h"
#include "..\Parsing\nodes.h"
#include "symbolTable.h"
#include "functionTable.h"
#include "commandTable.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for checking data types
//
// Version 2.5
// ----------------------------------------------------------
class DataTypingPhase : public Phase
{
private:
	SymbolTable * symbols;
	FunctionTable * functions;
	CommandTable * commands;
	bool changeMadeThisRound;
	bool setUnknownTypeNodesToDefault;

	void checkAndSetNodeDataType(AbstractNode * node, DataType type);
	void checkAndSetTreeDataType(AbstractNode * node, DataType type);
	void checkAndSetArgDataType(Command * cmd, int argNo, DataType type);
	void unifyTreeDataType(AbstractNode * node);
	void dataType_error(string);

public:
	DataTypingPhase() 
	{ 
		symbols = new SymbolTable(); 
		functions = new FunctionTable();
		commands = new CommandTable();
		changeMadeThisRound = false; 
		setUnknownTypeNodesToDefault = false;
	}

	SymbolTable * getSymbolTable();
	FunctionTable * getFunctionTable();
	CommandTable * getCommandTable();

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