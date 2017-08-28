//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the functionality to interpret a file
// -----------------------------------------------------------------
#include "parser.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This function displays an error to the user and terminates
// the program.
// @err_msg: The message to display.
//
// Version 5.0
// ----------------------------------------------------------
void Parser::syn_error(string type, string err_msg)
{
	int lineNo = scanner->getLineNo();
	cout << ((type == "") ? "" : type + " ") << "SYNTAX ERROR in file " << currFilePath << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

// ----------------------------------------------------------
// This function opens the SCF and checks for existance.
// @SCFPath: The path to the SCF.
//
// Version 5.0
// ----------------------------------------------------------
void Parser::open(string path)
{
	scanner->open(path);

	if (!scanner->good())
	{
		scanner->close();
		syn_error("", "File not found: " + path);
	}

	currFilePath = path;
}