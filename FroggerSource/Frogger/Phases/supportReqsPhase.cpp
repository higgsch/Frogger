//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program represents a visitor for gathering support code reqs.
// -----------------------------------------------------------------
#include "supportReqsPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function emits the includes and using statements.
// @ast: The representation of the input program.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::gatherRequirements(Language * language, ProgramStruct * prog)
{
	lang = language;

	prog->getPEFNode()->accept(this);

	int UDFCount = prog->getNumberOfUDFs();
	for (int index = 0; index < UDFCount; index++)
		prog->getUDFNode(index)->accept(this);
}

// ----------------------------------------------------------
// This function processes the include for a variable reference.
// @n: The node representing the variable.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(IdRefNode * n)
{
	if (n->getLexeme() == lang->SYM_ARGS)
		lang->argsUsed();
}

// ----------------------------------------------------------
// This function processes the include for a function call.
// @n: The node representing the statement.
//
// Version 5.0
// ----------------------------------------------------------
void SupportReqsPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	Routine * funct = n->getFunct();
	if (funct->equals(lang->builtInFunctions->FUNCT_TO_STRING))
		lang->toStringUsed();

	else if (funct->equals(lang->builtInFunctions->FUNCT_TO_ASCII))
		lang->toAsciiUsed();

	else if (funct->equals(lang->builtInFunctions->FUNCT_PARSE_DOUBLE))
		lang->parseDoubleUsed();

	else if (funct->equals(lang->builtInFunctions->FUNCT_ASCII_AT))
		lang->asciiAtUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_LENGTH))
		lang->lengthUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_RETRIEVE_DOUBLE))
		lang->retrieveDoubleUsed();

	else if (funct->equals(lang->builtInFunctions->FUNCT_RETRIEVE_STRING))
		lang->retrieveStringUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_RANDOM))
		lang->randomUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_READ))
		lang->readUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_ELEMENT_AT))
		lang->elementAtUsed();
	
	else if (funct->equals(lang->builtInFunctions->FUNCT_SIZE))
		lang->sizeUsed();
}

// ----------------------------------------------------------
// This function processes the include for a command call.
// @n: The node representing the statement.
//
// Version 5.0
// ----------------------------------------------------------
void SupportReqsPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Routine * cmd = n->getCmd();
	if (cmd->equals(lang->builtInCommands->CMD_DISPLAY_DBL))
		lang->displayDoubleUsed();
	
	else if (cmd->equals(lang->builtInCommands->CMD_DISPLAY_STR))
		lang->displayStringUsed();
	
	else if (cmd->equals(lang->builtInCommands->CMD_OPEN_INPUT))
		lang->openInputUsed();
	
	else if (cmd->equals(lang->builtInCommands->CMD_CLOSE_INPUT))
		lang->closeInputUsed();

	else if (cmd->equals(lang->builtInCommands->CMD_OPEN_OUTPUT))
		lang->openOutputUsed();
	
	else if (cmd->equals(lang->builtInCommands->CMD_CLOSE_OUTPUT))
		lang->closeOutputUsed();
	
	else if (cmd->equals(lang->builtInCommands->CMD_WRITE))
		lang->writeUsed();
}

// ----------------------------------------------------------
// This function processes the include for an addition 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(AddingNode * n)	
{ 
	n->visitAllChildren(this); 

	lang->addOpUsed();
}

// ----------------------------------------------------------
// This function processes the include for a modulus division 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(ModDivingNode * n)		
{	
	n->visitAllChildren(this);

	lang->modDivOpUsed();
}

// ----------------------------------------------------------
// This function processes the include for an integer division 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(IDivingNode * n)		
{
	n->visitAllChildren(this);

	lang->iDivOpUsed();
}

// ----------------------------------------------------------
// This function processes the include for a rootation 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(RootingNode * n)		
{
	n->visitAllChildren(this);

	lang->rootOpUsed();
}

// ----------------------------------------------------------
// This function processes the include for an exponentiation 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(ExpingNode * n)		
{
	n->visitAllChildren(this);

	lang->expOpUsed();
}