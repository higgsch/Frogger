// -----------------------------------------------------------------
// This is the header file for the Node class. 
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

// identification for which binary child a node is.
typedef enum node_sides{
	LEFT, RIGHT
} node_side;

// set of node categories
typedef enum node_types{
	RETRIEVING, DISPLAYING, ENDING,
	ASSIGNING, IDREF, STRINGCONSTING, 
	DBLCONSTING, ADDING, SUBING, MULING, DIVING,
	NOTING, LTING, GTING, EQING, LTEING, GTEING
} node_type;

// ----------------------------------------------------------
// This class provides the error function for all AST nodes.
//
// Version 2.0
// ----------------------------------------------------------
class Node
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
};