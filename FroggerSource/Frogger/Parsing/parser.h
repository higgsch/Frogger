// -----------------------------------------------------------------
// This is the header for the Parser class.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\Scanning\scanner.h"
#include "..\Scanning\token.h"
#include "..\DataStructures\Nodes\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code.
//
// Version 3.0
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

	void match(token_type);
	void syntax_error(string);

	Token next_token();

public:
	Parser();

	void open(string inFile);
	void close();

	ProgramNode* parse();
};