//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.4
// -----------------------------------------------------------------
// This program compiles a .fgr source file to c++ output.
// -----------------------------------------------------------------
#include "fgrFunctionC.h"
#include "..\Phases\phases.h"
#include "..\Parsing\FgrParser.h"
#include <iostream>
using namespace std;

// ----------------------------------------------------------
// This function drives a Frogger Function compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 4.4
// ----------------------------------------------------------
void FgrFunctionC::compileFunctionToAST(string inFile, FunctionTable * functs, CommandTable * cmds, UDFRecord * rec)
{
	buildAST(rec, inFile);
	
	//printAST(funct, "printableAST.txt");

	computeJumpToLineNumbers(rec);

	convertStrings(rec);
	checkDataTypes(rec, functs, cmds);
}

// ----------------------------------------------------------
// This function drives the parsing of the FROGGER file.
// @inFile: The .fgr file to open (from project directory).
//
// Version 4.2
// ----------------------------------------------------------
void FgrFunctionC::buildAST(UDFRecord * funct, string inFile)
{
	FGRParser *p = new FGRParser();

	p->open(inFile);
	funct->root = p->parse();
	p->close();

	delete p;
}

// ----------------------------------------------------------
// This function drives the computation of line numbers to 
// jump to for each line.
//
// Version 4.2
// ----------------------------------------------------------
void FgrFunctionC::computeJumpToLineNumbers(UDFRecord * funct)
{
	//Set line numbers
	LineNoPhase *lnp = new LineNoPhase();
	funct->root->accept(lnp);
	delete lnp;

	//calculate and set jump to line numbers
	SummationPhase *sp = new SummationPhase();
	funct->root->accept(sp);
	delete sp;
}

// ----------------------------------------------------------
// This function drives string conversion from FROGGER strings
// to C++ strings.
//
// Version 4.2
// ----------------------------------------------------------
void FgrFunctionC::convertStrings(UDFRecord * funct)
{
	StringConversionPhase *scp = new StringConversionPhase();
	funct->root->accept(scp);
	delete scp;
}

// ----------------------------------------------------------
// This function drives data typing.
//
// Version 4.4
// ----------------------------------------------------------
void FgrFunctionC::checkDataTypes(UDFRecord * ast, FunctionTable * functs, CommandTable * cmds)
{
	SymbolTable * symbols = new SymbolTable(lang, ast);
	DataTypingPhase *dtp = new DataTypingPhase(lang, functs, cmds, symbols);
	ast->root->accept(dtp);
	ast->symbols = symbols;
	delete dtp;
}

// ----------------------------------------------------------
// This function drives test output of the tree to a file.
// @outFile: The file that tree should be is sent to.
//
// Version 4.2
// ----------------------------------------------------------
void FgrFunctionC::printAST(UDFRecord * funct, string outFile)
{
	ofstream* out = new ofstream();
	out->open(outFile);

	PrintingPhase *pp = new PrintingPhase(out);

	pp->includeAsciiInfo();
	//pp->includeDataTypingInfo();
	//pp->includeLineNumberInfo();
	funct->root->accept(pp);

	delete pp;

	out->close();
	delete out;
}