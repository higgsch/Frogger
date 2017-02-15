// -----------------------------------------------------------------
// This is the header file for the Node class. 
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <iostream>
#include "..\..\Phases\phase.h"
#include "..\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class provides the error function for all AST nodes.
//
// Version 3.0
// ----------------------------------------------------------
class Node : public DataTyped
{
protected:
	// ----------------------------------------------------------
	// This function displays an error to the user and terminates
	// the program.
	// @err_msg: The message to display.
	//
	// Version 2.0
	// ----------------------------------------------------------
	void ast_error(string err_msg)
	{
		cout << "AST ERROR: " << err_msg << endl;
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}

	// ----------------------------------------------------------
	// This function safely allows a phase to visit a node.
	//
	// Version 3.0
	// ----------------------------------------------------------
	void visitNode(Phase* p, Node* n)
	{
		if (n != NULL && p != NULL)
			n->accept(p);
	}

public:
	virtual bool isTreeTyped() =0;

	virtual void visitAllChildren(Phase* p) =0;
	virtual void accept(Phase*) =0;
};