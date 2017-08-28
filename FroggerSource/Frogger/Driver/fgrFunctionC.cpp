//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program compiles a .fgr source file to c++ output.
// -----------------------------------------------------------------
#include "fgrFunctionC.h"
#include "..\Phases\FGRPhases\fgrPhases.h"
#include "..\Parsing\FgrParser.h"
#include <iostream>
using namespace std;

// ----------------------------------------------------------
// This function drives a Frogger Function compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 5.0
// ----------------------------------------------------------
void FgrFunctionC::compileFunction(UDFRecord * rec)
{
	buildAST(rec);
	
	//printAST(funct, "printableAST.txt");

	computeJumpToLineNumbers(rec);

	convertStrings(rec);
	checkDataTypes(rec);
	gatherRequirements(rec);
}

// ----------------------------------------------------------
// This function drives the parsing of the FROGGER file.
// @inFile: The .fgr file to open (from project directory).
//
// Version 5.0
// ----------------------------------------------------------
void FgrFunctionC::buildAST(UDFRecord * funct)
{
	FGRParser *p = new FGRParser();

	p->open(funct->filepath);
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
// Version 5.0
// ----------------------------------------------------------
void FgrFunctionC::checkDataTypes(UDFRecord * ast)
{
	SymbolTable * symbols = new SymbolTable(lang, ast);
	ast->visibleTables->syms->merge(symbols);
	FGRDataTypingPhase *dtp = new FGRDataTypingPhase(lang, progStruct, ast->visibleTables->functs, ast->visibleTables->cmds, ast->visibleTables->syms, ast->name);
	ast->root->accept(dtp);
	delete dtp;
}

// ----------------------------------------------------------
// This function drives requirements gathering.
//
// Version 5.0
// ----------------------------------------------------------
void FgrFunctionC::gatherRequirements(UDFRecord * ast)
{
	SupportReqsPhase * reqs = new SupportReqsPhase(lang);
	ast->root->accept(reqs);
	delete reqs;
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