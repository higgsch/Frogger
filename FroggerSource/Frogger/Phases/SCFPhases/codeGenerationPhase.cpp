//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program facilitates the FGRCodeGenerationPhase through 
// the SCF.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function generates the PEF code.
//
// Version 5.0
// ----------------------------------------------------------
void CodeGenerationPhase::printPEFCode(ProgramStruct * prog)
{
	UDFRecord * PEF = prog->PEF;
	fcgp->updateCurrentUDFName(PEF->name);
	PEF->root->accept(fcgp);

	string pefText = PEF->root->outputText;
	p->printString(lang->getPEFCode(PEF, pefText));
}