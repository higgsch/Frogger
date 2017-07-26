// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "phase.h"
#include "..\DataStructures\Nodes\nodes.h"
#include "..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for checking data types
//
// Version 4.2
// ----------------------------------------------------------
class DataTypingPhase : public Phase
{
private:
	Language * lang;
	SymbolTable * symbols;
	FunctionTable * functions;
	CommandTable * commands;
	bool changeMadeThisRound;
	bool setUnknownTypeNodesToDefault;

	void checkAndSetNodeDataType(AsciiNode * node, DataType type);
	void checkAndSetTreeDataType(TerminalNode * node, DataType type);
	void checkAndSetTreeDataType(UnaryNode * node, DataType type);
	void checkAndSetTreeDataType(BinaryNode * node, DataType type);
	void checkAndSetArgDataType(Routine * rout, int argNo, DataType type, int lineNo);
	void unifyTreeDataType(TerminalNode * node);
	void unifyTreeDataType(UnaryNode * node);
	void unifyTreeDataType(BinaryNode * node);
	void dataType_error(string msg, int line_no);

	void processDoubleOperator(BinaryOpNode * n);

public:
	DataTypingPhase(Language * language, FunctionTable * functs, SymbolTable * syms);

	SymbolTable * getSymbolTable() { return symbols; }
	FunctionTable * getFunctionTable() { return functions; }
	CommandTable * getCommandTable() { return commands; }

	void visit(ProgramNode * n);
	void visit(JmpStmtNode * n) { n->visitAllChildren(this); }
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n);
	void visit(AssigningNode * n);
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n);
	void visit(StringConstingNode * n) { checkAndSetNodeDataType(n, DT_STRING); }
	void visit(DoubleConstingNode * n) { checkAndSetNodeDataType(n, DT_DOUBLE); }
	void visit(AddingNode * n);
	void visit(SubingNode * n) { processDoubleOperator(n); }
	void visit(MulingNode * n) { processDoubleOperator(n); }
	void visit(DivingNode * n) { processDoubleOperator(n); }
	void visit(ModDivingNode * n) { processDoubleOperator(n); }
	void visit(IDivingNode * n) { processDoubleOperator(n); }
	void visit(RootingNode * n) { processDoubleOperator(n); }
	void visit(ExpingNode * n) { processDoubleOperator(n); }
	void visit(NotingNode * n) { n->visitOperand(this); }
	void visit(LTingNode * n) { processDoubleOperator(n); }
	void visit(GTingNode * n) { processDoubleOperator(n); }
	void visit(EQingNode * n) { processDoubleOperator(n); }
	void visit(LTEingNode * n) { processDoubleOperator(n); }
	void visit(GTEingNode * n) { processDoubleOperator(n); }
};