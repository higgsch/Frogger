// -----------------------------------------------------------------
// This is the header for the FGRParser class.
// -----------------------------------------------------------------
#pragma once

#include "parser.h"
#include <string>
#include "..\Scanning\fgrScanner.h"
#include "..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code.
//
// Version 5.0
// ----------------------------------------------------------
class FGRParser : Parser
{
private:
	FGRScanner scanner; //the scanning object that provides a Token stream
	ProgramNode* root; //the root of the AST

	//Program Organization
	void prog();
	ControlFlowNode* flowstmts();
	ControlFlowNode* flowstmt();
	ControlFlowNode* nestedflowstmt();

	//Control
	IfNode* ifstmt();
	AsciiNode* boolexp();
	BinaryOpNode* boolop();

	//Action Statements
	JmpStmtNode* jmpstmt();
	AsciiNode* arglist(int argNo, Routine* rout);

	//Expressions
	AsciiNode* expr();
	AsciiNode* addterm();
	AsciiNode* multerm();
	AsciiNode* typedterm(bool isEndFunction);
	AsciiNode* primary();

	//Operators
	BinaryOpNode* addop();
	BinaryOpNode* mulop();
	BinaryOpNode* expop();

	void syntax_error(string msg) { syn_error("", msg); }

public:
	FGRParser() : Parser(&scanner), root(new ProgramNode()) {}

	void open(string inFile);
	void close();

	ProgramNode* parse();
};