//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
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
void SupportReqsPhase::gatherRequirements(ProgramAST * ast)
{
	ast->getPEFNode()->accept(this);

	int UDFCount = ast->getNumberOfUDFs();
	for (int index = 0; index < UDFCount; index++)
		ast->getUDFNode(index)->accept(this);
}

// ----------------------------------------------------------
// This function processes the include for a variable reference.
// @n: The node representing the variable.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(IdRefNode * n)
{
	if (n->getLexeme() == SYMBOL_TEXT::ARGS.getText())
		VAR_DEF::ARGS.needed();
}

// ----------------------------------------------------------
// This function processes the include for a function call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	Function * funct = n->getFunct();
	if (funct->equals(FunctionTable::FUNCT_TO_STRING))
		FUNCT_DEF::TO_STRING.needed();

	else if (funct->equals(FunctionTable::FUNCT_TO_ASCII))
		FUNCT_DEF::TO_ASCII.needed();

	else if (funct->equals(FunctionTable::FUNCT_PARSE_DOUBLE))
		FUNCT_DEF::PARSE_DOUBLE.needed();

	else if (funct->equals(FunctionTable::FUNCT_ASCII_AT))
		FUNCT_DEF::ASCII_AT.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_LENGTH))
		FUNCT_DEF::LENGTH.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_RETRIEVE_DOUBLE))
		FUNCT_DEF::RETRIEVE_DOUBLE.needed();

	else if (funct->equals(FunctionTable::FUNCT_RETRIEVE_STRING))
		FUNCT_DEF::RETRIEVE_STRING.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_RANDOM))
		FUNCT_DEF::RANDOM.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_READ))
		FUNCT_DEF::READ.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_ELEMENT_AT))
		FUNCT_DEF::ELEMENT_AT.needed();
	
	else if (funct->equals(FunctionTable::FUNCT_SIZE))
		FUNCT_DEF::SIZE.needed();
}

// ----------------------------------------------------------
// This function processes the include for a command call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Command * cmd = n->getCmd();
	if (cmd->equals(CommandTable::CMD_DISPLAY_DBL))
		FUNCT_DEF::DISPLAY_DBL.needed();
	
	else if (cmd->equals(CommandTable::CMD_DISPLAY_STR))
		FUNCT_DEF::DISPLAY_STR.needed();
	
	else if (cmd->equals(CommandTable::CMD_OPEN_INPUT))
		FUNCT_DEF::OPEN_I.needed();
	
	else if (cmd->equals(CommandTable::CMD_CLOSE_INPUT))
		FUNCT_DEF::CLOSE_I.needed();

	else if (cmd->equals(CommandTable::CMD_OPEN_OUTPUT))
		FUNCT_DEF::OPEN_O.needed();
	
	else if (cmd->equals(CommandTable::CMD_CLOSE_OUTPUT))
		FUNCT_DEF::CLOSE_O.needed();
	
	else if (cmd->equals(CommandTable::CMD_WRITE))
		FUNCT_DEF::WRITE.needed();
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

	VAR_DEF::EMPTY_STRING.needed();
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

	IMPORT_STMT::MATH.needed();
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

	FUNCT_DEF::ROUND.needed();
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

	FUNCT_DEF::RT.needed();
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

	IMPORT_STMT::MATH.needed();
}