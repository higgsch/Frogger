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
	Token current_token; //the currently-selected Token
	Token lookahead[2]; //storage location for two lookahead Tokens
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

	void match(token_type);
	void syntax_error(string msg) { syn_error("", msg); }

	Token next_token();
	Token second_token();

public:
	FGRParser();

	void open(string inFile);
	void close();

	ProgramNode* parse();
};