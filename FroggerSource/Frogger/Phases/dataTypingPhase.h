// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phase.h"
#include "..\Parsing\nodes.h"
#include "tables.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for checking data types
//
// Version 3.0
// ----------------------------------------------------------
class DataTypingPhase : public Phase
{
private:
	SymbolTable * symbols;
	FunctionTable * functions;
	CommandTable * commands;
	bool changeMadeThisRound;
	bool setUnknownTypeNodesToDefault;

	void checkAndSetNodeDataType(AsciiNode * node, DataType type);
	void checkAndSetTreeDataType(TerminalNode * node, DataType type);
	void checkAndSetTreeDataType(UnaryNode * node, DataType type);
	void checkAndSetTreeDataType(BinaryNode * node, DataType type);
	void checkAndSetArgDataType(Command * cmd, int argNo, DataType type);
	void unifyTreeDataType(TerminalNode * node);
	void unifyTreeDataType(UnaryNode * node);
	void unifyTreeDataType(BinaryNode * node);
	void dataType_error(string);

public:
	DataTypingPhase();

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