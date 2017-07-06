// -----------------------------------------------------------------
// This is the header for the FGRParser class.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\Scanning\fgrScanner.h"
#include "..\Scanning\fgrToken.h"
#include "..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code.
//
// Version 4.0
// ----------------------------------------------------------
class FGRParser
{
private:
	FGRScanner scanner; //the scanning object that provides a Token stream
	FGRToken current_token; //the currently-selected Token
	FGRToken lookahead[1]; //storage location for one lookahead Token
	ProgramNode* root; //the root of the AST

	string currFileName;

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
	AsciiNode* arglist(int argNo, Command* cmd);

	//Expressions
	AsciiNode* expr();
	AsciiNode* addterm();
	AsciiNode* multerm();
	AsciiNode* typedterm();
	AsciiNode* primary();

	//Operators
	BinaryOpNode* addop();
	BinaryOpNode* mulop();
	BinaryOpNode* expop();

	void match(fgr_token_type);
	void syntax_error(string);

	FGRToken next_token();

public:
	FGRParser();

	void open(string inFile);
	void close();

	ProgramNode* parse();
};