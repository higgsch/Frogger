// -----------------------------------------------------------------
// This is the header for the Parser class.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\Scanning\scanner.h"
#include "..\Scanning\token.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code.
//
// Version 2.5
// ----------------------------------------------------------
class Parser
{
private:
	Scanner scanner; //the scanning object that provides a Token stream
	Token current_token; //the currently-selected Token
	Token lookahead[1]; //storage location for one lookahead Token
	ProgramNode* root; //the root of the AST

	//Program Organization
	void prog();
	ControlFlowNode* flowstmts();
	ControlFlowNode* flowstmt();
	ControlFlowNode* nestedflowstmt();

	//Control
	IfNode* ifstmt();
	BinaryOpNode* boolexp();
	BinaryOpNode* boolop();

	//Action Statements
	JmpStmtNode* jmpstmt();
	AbstractNode* arglist(int argNo, Command* cmd);

	//Expressions
	AbstractNode* expr();
	AbstractNode* addterm();
	AbstractNode* multerm();
	AbstractNode* typedterm();
	AbstractNode* primary();

	//Operators
	BinaryOpNode* addop();
	BinaryOpNode* mulop();
	BinaryOpNode* expop();

	void match(token_type);
	void syntax_error(string);

	Token next_token();

public:
	Parser();
	void open(string);
	ProgramNode* parse();
};