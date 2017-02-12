//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program compiles a .fgr source file to c++ output.
// -----------------------------------------------------------------
#include "compiler.h"
#include "..\Phases\LineNoPhase.h"
#include "..\Phases\summationPhase.h"
#include "..\Phases\stringConversionPhase.h"
#include "..\Phases\dataTypingPhase.h"
#include "..\Phases\codeGenerationPhase.h"
#include "..\Parsing\nodes.h"
using namespace std;

// ----------------------------------------------------------
// This function drives the compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::run(string inFile, string outFile)
{
	//Initialize the parser and open the file
	Parser p = Parser();
	p.open(inFile);
	ProgramNode * root = p.parse();
	p.close();

	LineNoPhase* lineP = new LineNoPhase();
	root->accept(lineP);

	//Compute goto line values and convert strings from frogger to c++
	root->accept(new SummationPhase(lineP->getLineCount()));
	root->accept(new StringConversionPhase());

	//Generate output code to the out stream
	root->accept(new DataTypingPhase());
	CodeGenerationPhase * codeGen = new CodeGenerationPhase();
	codeGen->open(outFile);
	root->accept(codeGen);
	codeGen->close();
}

// ----------------------------------------------------------
// The main function for the frogger compiler. Takes a .fgr 
// file and .cpp file from command line and runs the 
// compilation process.
// Note: Command line accepts .fgr filename then .cpp filename.
//
// Version 3.0
// ----------------------------------------------------------
int main(int argc, char* argv[])
{
	string inFile; //.fgr filename
	string outFile; //.cpp filename

	if (argc > 2)
	{ //filenames exist on command line
		inFile = argv[1];
		outFile = argv[2];

		cout << "Starting Compilation: " << inFile << " -> " << outFile << endl;
	}
	else
	{
		cout << "usage: compiler.exe <input filename> <output filename>" << endl;
		return 0;
	}

	Compiler c;
	c.run(inFile, outFile);

	cout << "\nPress Enter to Exit" << endl;
	getchar();

	return 0;
}