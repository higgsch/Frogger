//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
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
// Version 3.3
// ----------------------------------------------------------
IncludesSubPhase::IncludesSubPhase(ostream* outstream)
{
	out = outstream;

	isIOStreamImported = false;
	isMathImported = false;
	isStdLibImported = false;
	isTimeImported = false;
	isFStreamImported = false;

	hasRndNode = false;
	needsRoundFunct = false;
	needsStringToDouble = false;
	needsStringToAscii = false;
	needsIFile = false;
	needsOFile = false;
}

// ----------------------------------------------------------
// This function processes the include for a function call.
// @n: The node representing the statement.
//
// Version 3.3
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
	else if (funct->equals(FunctionTable::FUNCT_READ))
	{
		importFStream();

		needsIFile = true;
	}
}

// ----------------------------------------------------------
// This function processes the include for a command call.
// @n: The node representing the statement.
//
// Version 3.2
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
	else if (cmd->equals(CommandTable::CMD_OPEN_INPUT)
		|| cmd->equals(CommandTable::CMD_CLOSE_INPUT))
	{
		importFStream();

		needsIFile = true;
	}
	else if (cmd->equals(CommandTable::CMD_OPEN_OUTPUT)
		|| cmd->equals(CommandTable::CMD_CLOSE_OUTPUT)
		|| cmd->equals(CommandTable::CMD_WRITE))
	{
		importFStream();

		needsOFile = true;
	}
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
// This function emits the using statement.
//
// Version 3.3
// ----------------------------------------------------------
void IncludesSubPhase::emitUsingStatment()
{
	*out << "#include <string>;\n"
		<< "#include <vector>;\n"
		<< "using namespace std;\n\n";
}

// ----------------------------------------------------------
// This function emits the global support code (constants and
// functions).
//
// Version 4.0
// ----------------------------------------------------------
void IncludesSubPhase::emitSupportCode()
{
	emitArgVector();
	emitFileStreams();
	emitEmptyString();

	emitRoundFunction();
	emitStringToDoubleFunction();
	emitStringToAsciiFunction();
	emitElemAtFunction();
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

// ----------------------------------------------------------
// This function writes the include statment for the fstream
// library.
//
// Version 3.2
// ----------------------------------------------------------
void IncludesSubPhase::importFStream()
{
	if (!isFStreamImported)
	{
		*out << "#include <fstream>\n";
		isFStreamImported = true;
	}
}

// ----------------------------------------------------------
// This function emits the emptyString constant.
//
// Version 3.3
// ----------------------------------------------------------
void IncludesSubPhase::emitEmptyString()
{
	*out << "const string emptyString = \"\";\n\n";
}

// ----------------------------------------------------------
// This function emits the in_file and out_file objects.
//
// Version 4.0
// ----------------------------------------------------------
void IncludesSubPhase::emitFileStreams()
{

	if (needsIFile)
		*out << "ifstream in_file;\n";

	if (needsOFile)
		*out << "ofstream out_file;\n";
}

// ----------------------------------------------------------
// This function emits the round function.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::emitRoundFunction()
{
	if (needsRoundFunction())
		*out << "double round(double num) {\n"
			<< "\treturn (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);\n"
			<< "}\n\n";
}

// ----------------------------------------------------------
// This function emits the string to double conversion 
// function.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::emitStringToDoubleFunction()
{
	if (needsStringToDoubleFunction())
		*out << "double stringToDouble(string s) {\n"
			<< "\tif (isdigit(s[0]) || s[0] == '-')\n"
			<< "\t\treturn stod(s, NULL);\n"
			<< "\treturn 0;\n"
			<< "}\n\n";
}

// ----------------------------------------------------------
// This function emits the string to ascii conversion function.
//
// Version 3.0
// ----------------------------------------------------------
void IncludesSubPhase::emitStringToAsciiFunction()
{
	if (needsStringToAsciiFunction())
		*out << "double stringToAscii(string s, int loc) {\n"
			<< "\tif (loc < 0 || loc >= s.length())\n"
			<< "\t\treturn 0;\n"
			<< "\treturn s.at(loc);\n"
			<< "}\n\n";
}

// ----------------------------------------------------------
// This function emits the elemAt function.
//
// Version 3.3
// ----------------------------------------------------------
void IncludesSubPhase::emitElemAtFunction()
{
	*out << "string elemAt(vector<string> v, int index) {\n"
		<< "\tif (index < 0 || index >= v.size())\n"
		<< "\t\treturn \"\";\n"
		<< "\treturn v[index];\n"
		<< "}\n\n";
}

// ----------------------------------------------------------
// This function emits the vector of argument strings.
//
// Version 4.0
// ----------------------------------------------------------
void IncludesSubPhase::emitArgVector()
{
	*out << "vector<string> args;\n";
}