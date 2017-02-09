//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.3
// -----------------------------------------------------------------
// This program represents a visitor for generating include statements.
// -----------------------------------------------------------------
#include "includesSubPhase.h"
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
}

// ----------------------------------------------------------
// This function processes the include for a line of code.
// @n: The node representing the line.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(JmpStmtNode * n)		
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an if statement.
// @n: The node representing the statement.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(IfNode * n)			
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(RetrievalNode * n)		
{
	if (!isIOStreamImported)
	{
		*out << "#include <iostream>\n";
		isIOStreamImported = true;
	}
}

// ----------------------------------------------------------
// This function processes the include for a display statement.
// @n: The node representing the display statement.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(DisplayingNode * n)	
{
	if (!isIOStreamImported)
	{
		*out << "#include <iostream>\n";
		isIOStreamImported = true;
	}

	n->visitLeftChild(this);
}

// ----------------------------------------------------------
// This function processes the include for a random statement.
// @n: The node representing the random statement.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(RandomingNode * n)		
{
	if (!isStdLibImported)
	{
		*out << "#include <stdlib.h>\n";
		isStdLibImported = true;
	}

	if (!isTimeImported)
	{
		*out << "#include <time.h>\n";
		isTimeImported = true;
	}

	hasRndNode = true;
}

// ----------------------------------------------------------
// This function processes the include for an assignment 
// statement.
// @n: The node representing the statement.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(AssigningNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a string literal.
// @n: The node representing the string.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(StringConstingNode * n)
{
	if (!isStringImported)
	{
		*out << "#include <string>\n";
		isStringImported = true;
	}
}

// ----------------------------------------------------------
// This function processes the include for an addition operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(AddingNode * n)
{
	//include string for concatenation
	if (!isStringImported)
	{
		*out << "#include <string>\n";
		isStringImported = true;
	}
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a subtraction operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(SubingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a multiplication operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(MulingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a division operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(DivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a modulus division 
// operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(ModDivingNode * n)		
{
	if (!isMathImported)
	{
		*out << "#include <math.h>\n";
		isMathImported = true;
	}
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an integer division 
// operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(IDivingNode * n)		
{
	if (!isMathImported)
	{
		*out << "#include <math.h>\n";
		isMathImported = true;
	}
	
	n->visitAllChildren(this);

	needsRoundFunct = true;
}

// ----------------------------------------------------------
// This function processes the include for a rootation 
// operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(RootingNode * n)		
{
	if (!isMathImported)
	{
		*out << "#include <math.h>\n";
		isMathImported = true;
	}
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an exponentiation 
// operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(ExpingNode * n)		
{
	if (!isMathImported)
	{
		*out << "#include <math.h>\n";
		isMathImported = true;
	}
	
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a not operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(NotingNode * n)
{
	n->visitLeftChild(this);
}

// ----------------------------------------------------------
// This function processes the include for a less than 
// comparison operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(LTingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a greater than 
// comparison operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(GTingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for an equivalence 
// comparison operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(EQingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a less than or 
// equal comparison operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(LTEingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes the include for a greater than or 
// equal comparison operation.
// @n: The node representing the operation.
//
// Version 2.2
// ----------------------------------------------------------
void IncludesSubPhase::visit(GTEingNode * n)
{
	n->visitAllChildren(this);
}