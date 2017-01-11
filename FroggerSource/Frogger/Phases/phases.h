// -----------------------------------------------------------------
// This is the header for the Phase class.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <iostream>
using namespace std;

//forward declarations
class AbstractNode;
class BinaryOpNode;
class ControlFlowNode;
class JmpStmtNode;
class IfNode;
class ProgramNode;
class RetrievalNode;
class DisplayingNode;
class EndingNode;
class IdRefNode;
class AssigningNode;
class StringConstingNode;
class DoubleConstingNode;
class AddingNode;
class SubingNode;
class MulingNode;
class DivingNode;
class NotingNode;
class LTingNode;
class GTingNode;
class EQingNode;
class LTEingNode;
class GTEingNode;

// ----------------------------------------------------------
// This class provides the base of the visitor inheritance for
// the AST intermediate representation. 
//
// Version 2.0
// ----------------------------------------------------------
class Phase
{
public:
	//virtual visit functions required by visitor pattern
	virtual void visit(JmpStmtNode * n) = 0;
	virtual void visit(IfNode * n) = 0;
	virtual void visit(RetrievalNode * n) = 0;
	virtual void visit(DisplayingNode * n) = 0;
	virtual void visit(EndingNode * n) = 0;
	virtual void visit(IdRefNode * n) = 0;
	virtual void visit(AssigningNode * n) = 0;
	virtual void visit(StringConstingNode * n) = 0;
	virtual void visit(DoubleConstingNode * n) = 0;
	virtual void visit(AddingNode * n) = 0;
	virtual void visit(SubingNode * n) = 0;
	virtual void visit(MulingNode * n) = 0;
	virtual void visit(DivingNode * n) = 0;
	virtual void visit(NotingNode * n) = 0;
	virtual void visit(LTingNode * n) = 0;
	virtual void visit(GTingNode * n) = 0;
	virtual void visit(EQingNode * n) = 0;
	virtual void visit(LTEingNode * n) = 0;
	virtual void visit(GTEingNode * n) = 0;

	// ----------------------------------------------------------
	// This function displays an error to the user and terminates
	// the program.
	// @msg: The message to display.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void semantic_error(string msg)
	{
		cout << "SEMANTIC ERROR: " << msg << endl;
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}
};