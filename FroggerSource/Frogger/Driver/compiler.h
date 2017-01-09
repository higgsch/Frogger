// -----------------------------------------------------------------
// This is the header file for the Compiler class
// -----------------------------------------------------------------
#pragma once

#include <string>
#include "..\Parsing\parser.h"
using namespace std;

class ProgramNode;

// ----------------------------------------------------------
// This class is the driver class for the compiler program.
// It can be used to compile a .fgr source file to c++ output.
//
// Version 2.0
// ----------------------------------------------------------
class Compiler
{
private:
	Parser p;
	ProgramNode * root; //A pointer to the root of the AST for input source

public:
	Compiler();
	void run(string, ostream*);
};