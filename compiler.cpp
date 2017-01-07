//                      Christopher Higgs
//                      CS 6820 - 7:30 am
//                      Final Project
//                      Dr. Rague
//                      Due: 12/10/16
//                      Version: 1.0
// -----------------------------------------------------------------
// This program compiles a .fgr source file to c++ output.
// -----------------------------------------------------------------
#include "compiler.h"
#include "summationPhase.h"
#include "stringConversionPhase.h"
#include "codeGenerationPhase.h"
#include "parser.h"
#include "nodes.h"
using namespace std;

// ----------------------------------------------------------
// This function drives the compilation process.
// @inFile: The .fgr file to open (from project directory).
// @out: The output stream that output source is sent to.
//
// Version 1.0
// ----------------------------------------------------------
void Compiler::run(string inFile, ostream * out)
{
	//Initialize the parser and open the file
	Parser p = Parser();
	p.open(inFile);

	//Initiate the compilation process
	ProgramNode * root = p.parse();

	//Compute goto line values and convert strings from frogger to c++
	root->traverseNodes(new SummationPhase(root->getLineCount()));
	root->traverseNodes(new StringConversionPhase());

	//Generate output code to the out stream
	root->traverseNodes(new CodeGenerationPhase(out, root));
	*out << "\n}" << endl; //close the c++ main function

	//Test the AST
	//root->printNodes(out);
}

// ----------------------------------------------------------
// The main function for the frogger compiler. Takes a .fgr 
// file and .cpp file from command line and runs the 
// compilation process.
// Note: Command line accepts .fgr filename then .cpp filename.
//
// Version 1.0
// ----------------------------------------------------------
int main(int argc, char* argv[])
{
	string inFile; //.fgr filename
	string outFile; //.cpp filename

	ostream * out;
	ofstream outF;

	if (argc > 2)
	{ //filenames exist on command line
		inFile = argv[1];
		outFile = argv[2];

		outF = ofstream();
		outF.open(outFile);
		out = &outF;

		cout << "Starting Compilation: " << inFile << " -> " << outFile << endl;
	}
	else
	{
		cout << "usage: compiler.exe <input filename> <output filename>" << endl;
		return 0;
	}

	Compiler c;
	c.run(inFile, out);

	outF.close();

	cout << "\nPress Enter to Exit" << endl;
	getchar();

	return 0;
}