//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents a visitor for gathering support code reqs.
// -----------------------------------------------------------------
#include "supportReqsSubPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function emits the includes and using statements.
// @ast: The representation of the input program.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::gatherRequirements(ProgramAST * ast)
{
	ast->PEF->root->accept(this);

	int UDFCount = ast->UDFs->size();
	for (int index = 0; index < UDFCount; index++)
	{
		(*(ast->UDFs))[index]->root->accept(this);
	}
}

// ----------------------------------------------------------
// This function processes the include for a variable reference.
// @n: The node representing the variable.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(IdRefNode * n)
{
	if (n->getLexeme() == "args")
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(IMPORT_STMT::VECTOR);
		setAsNeeded(VAR_DEF::ARGS);
		setAsNeeded(INIT_STMT::ARGS);
	}
}

// ----------------------------------------------------------
// This function processes the include for a function call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);

	Function * funct = n->getFunct();
	if (funct->equals(FunctionTable::FUNCT_TO_STRING))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::TO_STRING);
	}
	else if (funct->equals(FunctionTable::FUNCT_TO_ASCII))
		setAsNeeded(FUNCT_DEF::TO_ASCII);
	else if (funct->equals(FunctionTable::FUNCT_PARSE_DOUBLE))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::PARSE_DOUBLE);
	}
	else if (funct->equals(FunctionTable::FUNCT_ASCII_AT))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::ASCII_AT);
	}
	else if (funct->equals(FunctionTable::FUNCT_LENGTH))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::LENGTH);
	}
	else if (funct->equals(FunctionTable::FUNCT_RETRIEVE_DOUBLE))
	{
		setAsNeeded(IMPORT_STMT::IO_STREAM);
		setAsNeeded(FUNCT_DEF::RETRIEVE_DOUBLE);
	}
	else if (funct->equals(FunctionTable::FUNCT_RETRIEVE_STRING))
	{
		setAsNeeded(IMPORT_STMT::IO_STREAM);
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::RETRIEVE_STRING);
	}
	else if (funct->equals(FunctionTable::FUNCT_RANDOM))
	{
		setAsNeeded(IMPORT_STMT::STD_LIB);
		setAsNeeded(IMPORT_STMT::TIME);
		setAsNeeded(FUNCT_DEF::RANDOM);
		setAsNeeded(INIT_STMT::RANDOM);
	}
	else if (funct->equals(FunctionTable::FUNCT_READ))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(VAR_DEF::I_FILE);
		setAsNeeded(INIT_STMT::I_FILE);
		setAsNeeded(FUNCT_DEF::READ);
	}
	else if (funct->equals(FunctionTable::FUNCT_ELEMENT_AT))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(IMPORT_STMT::VECTOR);
		setAsNeeded(FUNCT_DEF::ELEMENT_AT);
	}
	else if (funct->equals(FunctionTable::FUNCT_SIZE))
	{
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(IMPORT_STMT::VECTOR);
		setAsNeeded(FUNCT_DEF::SIZE);
	}
}

// ----------------------------------------------------------
// This function processes the include for a command call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);

	Command * cmd = n->getCmd();
	if (cmd->equals(CommandTable::CMD_DISPLAY_DBL))
	{
		setAsNeeded(IMPORT_STMT::IO_STREAM);
		setAsNeeded(FUNCT_DEF::DISPLAY_DBL);
	}
	else if (cmd->equals(CommandTable::CMD_DISPLAY_STR))
	{
		setAsNeeded(IMPORT_STMT::IO_STREAM);
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(FUNCT_DEF::DISPLAY_STR);
	}
	else if (cmd->equals(CommandTable::CMD_OPEN_INPUT))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(VAR_DEF::I_FILE);
		setAsNeeded(INIT_STMT::I_FILE);
		setAsNeeded(FUNCT_DEF::OPEN_I);
	}
	else if (cmd->equals(CommandTable::CMD_CLOSE_INPUT))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(VAR_DEF::I_FILE);
		setAsNeeded(INIT_STMT::I_FILE);
		setAsNeeded(FUNCT_DEF::CLOSE_I);
	}
	else if (cmd->equals(CommandTable::CMD_OPEN_OUTPUT))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(VAR_DEF::O_FILE);
		setAsNeeded(INIT_STMT::O_FILE);
		setAsNeeded(FUNCT_DEF::OPEN_O);
	}
	else if (cmd->equals(CommandTable::CMD_CLOSE_OUTPUT))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(VAR_DEF::O_FILE);
		setAsNeeded(INIT_STMT::O_FILE);
		setAsNeeded(FUNCT_DEF::CLOSE_O);
	}
	else if (cmd->equals(CommandTable::CMD_WRITE))
	{
		setAsNeeded(IMPORT_STMT::F_STREAM);
		setAsNeeded(IMPORT_STMT::STRING);
		setAsNeeded(VAR_DEF::O_FILE);
		setAsNeeded(INIT_STMT::O_FILE);
		setAsNeeded(FUNCT_DEF::WRITE);
	}
}

// ----------------------------------------------------------
// This function processes the include for an addition 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(AddingNode * n)	
{ 
	n->visitAllChildren(this); 

	setAsNeeded(IMPORT_STMT::STRING);
	setAsNeeded(VAR_DEF::EMPTY_STRING);
}

// ----------------------------------------------------------
// This function processes the include for a modulus division 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(ModDivingNode * n)		
{	
	n->visitAllChildren(this);

	setAsNeeded(IMPORT_STMT::MATH);
}

// ----------------------------------------------------------
// This function processes the include for an integer division 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(IDivingNode * n)		
{
	n->visitAllChildren(this);

	setAsNeeded(IMPORT_STMT::MATH);
	setAsNeeded(FUNCT_DEF::ROUND);
}

// ----------------------------------------------------------
// This function processes the include for a rootation 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(RootingNode * n)		
{
	n->visitAllChildren(this);

	setAsNeeded(IMPORT_STMT::MATH);
	setAsNeeded(FUNCT_DEF::RT);
}

// ----------------------------------------------------------
// This function processes the include for an exponentiation 
// operation.
// @n: The node representing the operation.
//
// Version 4.2
// ----------------------------------------------------------
void SupportReqsSubPhase::visit(ExpingNode * n)		
{
	n->visitAllChildren(this);

	setAsNeeded(IMPORT_STMT::MATH);
}