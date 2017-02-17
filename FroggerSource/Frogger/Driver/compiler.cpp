//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program compiles a .fgr source file to c++ output.
// -----------------------------------------------------------------
#include "compiler.h"
#include "..\Phases\phases.h"
#include "..\Parsing\parser.h"
#include <iostream>
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
	buildAST(inFile);
	
	//printAST("printableAST.txt");

	setLineNumbers();
	computeJumpToLineNumbers(); //Depends on line numbers

	convertStrings();
	checkDataTypes();
	
	emitCode(outFile);
}

// ----------------------------------------------------------
// This function drives the parsing of the FROGGER file.
// @inFile: The .fgr file to open (from project directory).
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::buildAST(string inFile)
{
	Parser *p = new Parser();

	p->open(inFile);
	root = p->parse();
	p->close();

	delete p;
}

// ----------------------------------------------------------
// This function drives the setting of line numbers to each 
// line (not the jump to line, but the line number itself).
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::setLineNumbers()
{
	LineNoPhase *lnp = new LineNoPhase();
	root->accept(lnp);
	delete lnp;
}

// ----------------------------------------------------------
// This function drives the computation of line numbers to 
// jump to for each line.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::computeJumpToLineNumbers()
{
	SummationPhase *sp = new SummationPhase();
	root->accept(sp);
	delete sp;
}

// ----------------------------------------------------------
// This function drives string conversion from FROGGER strings
// to C++ strings.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::convertStrings()
{
	StringConversionPhase *scp = new StringConversionPhase();
	root->accept(scp);
	delete scp;
}

// ----------------------------------------------------------
// This function drives data typing.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::checkDataTypes()
{
	DataTypingPhase *dtp = new DataTypingPhase();
	root->accept(dtp);
	symbols = dtp->getSymbolTable();
	delete dtp;
}

// ----------------------------------------------------------
// This function drives code generation.
// @outFile: The file that output source is sent to.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::emitCode(string outFile)
{
	CodeGenerationPhase *cgp = new CodeGenerationPhase(symbols);

	cgp->open(outFile);
	root->accept(cgp);
	cgp->close();

	delete cgp;
}

// ----------------------------------------------------------
// This function drives test output of the tree to a file.
// @outFile: The file that tree should be is sent to.
//
// Version 3.0
// ----------------------------------------------------------
void Compiler::printAST(string outFile)
{
	ofstream* out = new ofstream();
	out->open(outFile);

	PrintingPhase *pp = new PrintingPhase(out);

	pp->includeAsciiInfo();
	//pp->includeDataTypingInfo();
	//pp->includeLineNumberInfo();
	root->accept(pp);

	delete pp;

	out->close();
	delete out;
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

	if (argc != 3)
	{ 
		cout << "usage: compiler.exe <input filename> <output filename>" << endl;
		return 0;
	}
	
	//filenames exist on command line
	inFile = argv[1];
	outFile = argv[2];

	cout << "Starting Compilation: " << inFile << " -> " << outFile << endl;
	
	Compiler c;
	c.run(inFile, outFile);

	cout << "\nPress Enter to Exit" << endl;
	getchar();

	return 0;
}