// -----------------------------------------------------------------
// This is the header for the Parser class.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "scanner.h"
#include "token.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This class parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code.
//
// Version 1.2
// ----------------------------------------------------------
class Parser
{
private:
	Scanner scanner; //the scanning object that provides a Token stream
	Token current_token; //the currently-selected Token
	Token lookahead[1]; //storage location for one lookahead Token
	ProgramNode* root; //the root of the AST

	void prog();
	void lines();
	AbstractNode* line();
	AbstractNode* val();
	AbstractNode* dblval();
	AbstractNode* addterm();
	AbstractNode* multerm();
	BinaryOpNode* addop();
	BinaryOpNode* mulop();

	void match(token_type);
	void syntax_error(string);

	Token next_token();

public:
	Parser();
	void open(string);
	ProgramNode* parse();
};