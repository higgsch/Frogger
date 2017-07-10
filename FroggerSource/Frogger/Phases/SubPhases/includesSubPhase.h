// -----------------------------------------------------------------
// This is the header for the ImportsSubPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\phase.h"
#include "..\..\DataStructures\Nodes\nodes.h"
#include "..\..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a visitor for generating import
// statements as a subphase of the CodeGenerationPhase.
//
// Version 4.2
// ----------------------------------------------------------
class IncludesSubPhase : public Phase
{
private:
	ostream* out; // the output stream to print to
	bool isIOStreamImported; // flag for <iostream>
	bool isMathImported; // flag for <math.h>
	bool isStdLibImported; // flag for <stdlib.h>
	bool isTimeImported; // flag for <time.h>
	bool isFStreamImported; // flag for <fstream>

	bool hasRndNode; // flag for seeding
	bool needsRoundFunct; // flag for round() creation
	bool needsIFile; // flag for in_file
	bool needsOFile; // flag for out_file

	bool needsToString; //flag for toString() creation
	bool needsToAscii; //flag for toAscii() creation
	bool needsParseDouble; // flag for parseDouble() creation
	bool needsAsciiAt; // flag for asciiAt() creation
	bool needsLength; // flag for length() creation
	bool needsRetrieveDouble; //flag for retrieveDouble() creation
	bool needsRetrieveString; //flag for retrieveString() creation
	bool needsRandom; //flag for random() creation
	bool needsRead; //flag for read() creation
	bool needsElementAt; //flag for elementAt() creation
	bool needsSize; //flag for size() creation

	bool needsDisplay; //flag for display() creation
	bool needsOpenI; //flag for openI() creation
	bool needsOpenO; //flag for openO() creation
	bool needsWrite; //flag for write() creation
	bool needsCloseI; //flag for closeI() creation
	bool needsCloseO; //flag for closeO() creation

	void importIOStream();
	void importMath();
	void importStdLib();
	void importTime();
	void importFStream();

	void emitEmptyString();
	void emitFileStreams();
	void emitRoundFunction();
	void emitArgVector();
	void emitRtFunction();

public:
	IncludesSubPhase(ostream*);

	void emitIncludesStatements(ProgramAST * progAST);

	void visit(ProgramNode * n) { n->visitAllChildren(this); }
	void visit(JmpStmtNode * n) { n->visitAllChildren(this); }
	void visit(IfNode * n) { n->visitAllChildren(this); }
	void visit(IdRefNode * n){}
	void visit(AssigningNode * n) { n->visitAllChildren(this); }
	void visit(FunctionCallNode * n);
	void visit(CommandCallNode * n);
	void visit(ArgListNode * n) { n->visitAllChildren(this); }
	void visit(StringConstingNode * n){}
	void visit(DoubleConstingNode * n){}
	void visit(AddingNode * n) { n->visitAllChildren(this); }
	void visit(SubingNode * n) { n->visitAllChildren(this); }
	void visit(MulingNode * n) { n->visitAllChildren(this); }
	void visit(DivingNode * n) { n->visitAllChildren(this); }
	void visit(ModDivingNode * n);
	void visit(IDivingNode * n);
	void visit(RootingNode * n);
	void visit(ExpingNode * n);
	void visit(NotingNode * n) { n->visitOperand(this); }
	void visit(LTingNode * n) { n->visitAllChildren(this); }
	void visit(GTingNode * n) { n->visitAllChildren(this); }
	void visit(EQingNode * n) { n->visitAllChildren(this); }
	void visit(LTEingNode * n) { n->visitAllChildren(this); }
	void visit(GTEingNode * n) { n->visitAllChildren(this); }

	void emitUsingStatment();
	void emitSupportCode();

	bool hasRandomNode(){ return hasRndNode; }
	bool needsRoundFunction(){ return needsRoundFunct; }
	bool needsInputFile() { return needsIFile; }
	bool needsOutputFile() { return needsOFile; }
	
	bool needsToStringFunction() { return needsToString; }
	bool needsToAsciiFunction() { return needsToAscii; }
	bool needsParseDoubleFunction() { return needsParseDouble; }
	bool needsAsciiAtFunction() { return needsAsciiAt; }
	bool needsLengthFunction() { return needsLength; } 
	bool needsRetrieveDoubleFunction() { return needsRetrieveDouble; }
	bool needsRetrieveStringFunction() { return needsRetrieveString; }
	bool needsRandomFunction() { return needsRandom; }
	bool needsReadFunction() { return needsRead; } 
	bool needsElementAtFunction() { return needsElementAt; }
	bool needsSizeFunction() { return needsSize; } 

	bool needsDisplayCommand() { return needsDisplay; }
	bool needsOpenICommand() { return needsOpenI; } 
	bool needsOpenOCommand() { return needsOpenO; } 
	bool needsWriteCommand() { return needsWrite; } 
	bool needsCloseICommand() { return needsCloseI; }
	bool needsCloseOCommand() { return needsCloseO; }
};