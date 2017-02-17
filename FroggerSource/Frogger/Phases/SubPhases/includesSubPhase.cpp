//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for generating include statements.
// -----------------------------------------------------------------
#include "includesSubPhase.h"
#include "..\..\DataStructures\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This constructor generates an IncludesSubPhase for the
// given output stream and AST.
// @outstream: The output stream to print to.
//
// Version 2.2
// ----------------------------------------------------------
IncludesSubPhase::IncludesSubPhase(ostream* outstream)
{
	out = outstream;

	isStringImported = false;
	isIOStreamImported = false;
	isMathImported = false;
	isStdLibImported = false;
	isTimeImported = false;

	hasRndNode = false;
	needsRoundFunct = false;
	needsStringToDouble = false;
	needsStringToAscii = false;
}

// ----------------------------------------------------------
// This function processes the include for a function call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	Function * funct = n->getFunct();
	if (funct->equals(FunctionTable::FUNCT_PARSE_DOUBLE))
	{
		needsStringToDouble = true;
	}
	else if (funct->equals(FunctionTable::FUNCT_ASCII_AT))
	{
		needsStringToAscii = true;
	}
	else if (funct->equals(FunctionTable::FUNCT_RETRIEVE_DOUBLE) 
		|| funct->equals(FunctionTable::FUNCT_RETRIEVE_STRING))
	{
		importIOStream();
	}
	else if (funct->equals(FunctionTable::FUNCT_RANDOM))
	{
		importStdLib();
		importTime();

		hasRndNode = true;
	}

	if (funct->equals(FunctionTable::FUNCT_PARSE_DOUBLE) 
		|| funct->equals(FunctionTable::FUNCT_ASCII_AT))
	{
		importString();
	}
}

// ----------------------------------------------------------
// This function processes the include for a command call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Command * cmd = n->getCmd();
	if (cmd->equals(CommandTable::CMD_DISPLAY_DBL)
		|| cmd->equals(CommandTable::CMD_DISPLAY_STR))
	{
		importIOStream();
	}
}

// ----------------------------------------------------------
// This function processes the include for a string literal.
// @n: The node representing the string.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(StringConstingNode * n)
{
	importString();
}

// ----------------------------------------------------------
// This function processes the include for an addition operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(AddingNode * n)
{
	//include string for concatenation
	importString();
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a modulus division 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(ModDivingNode * n)		
{
	importMath();
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an integer division 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(IDivingNode * n)		
{
	importMath();
	
	n->visitAllChildren(this);

	needsRoundFunct = true;
}

// ----------------------------------------------------------
// This function processes the include for a rootation 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(RootingNode * n)		
{
	importMath();
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an exponentiation 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::visit(ExpingNode * n)		
{
	importMath();
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function writes the include statment for the string
// library.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::importString()
{
	if (!isStringImported)
	{
		*out << "#include <string>\n";
		isStringImported = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the iostream
// library.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::importIOStream()
{
	if (!isIOStreamImported)
	{
		*out << "#include <iostream>\n";
		isIOStreamImported = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the math
// library.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::importMath()
{
	if (!isMathImported)
	{
		*out << "#include <math.h>\n";
		isMathImported = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the stdlib
// library.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::importStdLib()
{
	if (!isStdLibImported)
	{
		*out << "#include <stdlib.h>\n";
		isStdLibImported = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the time
// library.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::importTime()
{
	if (!isTimeImported)
	{
		*out << "#include <time.h>\n";
		isTimeImported = true;
	}
}