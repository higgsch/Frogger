// -----------------------------------------------------------------
// This is the header file for the AbstractNode class. 
// -----------------------------------------------------------------
#pragma once

#include "..\node.h"
using namespace std;

class Phase;

// ----------------------------------------------------------
// This class provides the base of the node inheritance for
// the AST intermediate representation. Supports the visitor
// pattern for a Phase.
//
// Version 3.0
// ----------------------------------------------------------
class AsciiNode : public Node
{
protected:
	string lexeme; // the string representation of the node value
	int ascii_addition; // the value this node contributes to the Stmt's ascii
	int parenNestCount; // the number of parens surrounding this node directly

public:
	AsciiNode() { lexeme = ""; ascii_addition = 0; parenNestCount = 0; }
	~AsciiNode() {}

	string getLexeme() { return lexeme; }
	void setLexeme(string lex) { lexeme = lex; }

	void setAscii(int ascii) { ascii_addition = ascii; }
	int getAscii() { return ascii_addition; }

	void addParenNesting() { parenNestCount++; }
	int  getParenNesting() { return parenNestCount; }
};