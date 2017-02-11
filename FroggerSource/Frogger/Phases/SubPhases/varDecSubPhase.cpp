//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.5
// -----------------------------------------------------------------
// This program represents a visitor for generating variable
// declarations as a subphase of the CodeGenerationPhase.
// -----------------------------------------------------------------
#include "varDecSubPhase.h"
using namespace std;

// ----------------------------------------------------------
// This constructor generates a VarDecSubPhase for the
// given output stream.
// @outstream: The output stream to print to.
//
// Version 2.5
// ----------------------------------------------------------
VarDecSubPhase::VarDecSubPhase(ostream* outstream, int tabCount)
{
	list = NULL;
	out = outstream;
	currStmtDblTempCount = 0;
	currStmtStrTempCount = 0;
	lineDblTempMax = 0;
	lineStrTempMax = 0;
	indentDepth = tabCount;
}

// ----------------------------------------------------------
// This function adds the given variable to the symbol table.
// @id: The variable to add.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::addToList(string id)
{
	if (list == NULL)
	{ //this is the first variable in the list
		list = new VarList();
		list->id = id;
		list->tail = NULL;
		return;
	}
	
	//navigate to the last variable in the list
	VarList* curr = list;
	while (curr->tail != NULL)
		curr = curr->tail;

	//add the variable
	curr->tail = new VarList();
	VarList* currtail = curr->tail;
	currtail->id = id;
	currtail->tail = NULL;
}

// ----------------------------------------------------------
// This function returns if the given variable is in the 
// symbol table.
// @id: The variable to check for.
//
// Version 1.0
// ----------------------------------------------------------
bool VarDecSubPhase::isInList(string id)
{
	if (list == NULL)
		return false;

	//traverse the list
	VarList* curr = list;
	while (curr->tail != NULL)
	{
		if (!curr->id.compare(id))
			return true;
		curr = curr->tail;
	}

	if (!curr->id.compare(id))
		return true;
	return false;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::visit(JmpStmtNode * n)
{
	currStmtDblTempCount = 0;
	currStmtStrTempCount = 0;

	n->visitThisStmt(this);

	//update the temporary counters
	if (currStmtDblTempCount > lineDblTempMax)
		lineDblTempMax = currStmtDblTempCount;
	if (currStmtStrTempCount > lineStrTempMax)
		lineStrTempMax = currStmtStrTempCount;

	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::visit(IfNode * n)
{
	currStmtDblTempCount = 0;
	currStmtStrTempCount = 0;

	n->visitBoolExp(this);

	//update the temporary counters
	if (currStmtDblTempCount > lineDblTempMax)
		lineDblTempMax = currStmtDblTempCount;
	if (currStmtStrTempCount > lineStrTempMax)
		lineStrTempMax = currStmtStrTempCount;

	n->visitTrueStmt(this);
	n->visitFalseStmt(this);
	
	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 2.3
// ----------------------------------------------------------
void VarDecSubPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();
	DataType type = n->getDataType();

	if (!isInList(id))
	{
		addToList(id);
		//emit variable declaration and initialization
		if (type == DT_DOUBLE)
			*out << indent() << "double _" << id << " = 0;\n";
		else if (type == DT_STRING)
			*out << indent() << "string _" << id << " = \"\";\n";
		else
			*out << indent() << "Not_Defined _" << id << " = NULL;\n";
	}
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(AssigningNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::visit(FunctionCallNode * n)
{
	n->visitAllChildren(this);
	
	Function* funct = n->getFunct();
	if (!funct->isUserDefined())
	{
		string name = funct->name;
		if (name == "retrieveDouble")
		{
			currStmtDblTempCount++;
		}
		else if (name == "retrieveString")
		{
			currStmtStrTempCount++;
		}
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::visit(CommandCallNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 2.4
// ----------------------------------------------------------
void VarDecSubPhase::visit(ArgListNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(AddingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(SubingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(MulingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(DivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(ModDivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(IDivingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(RootingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void VarDecSubPhase::visit(ExpingNode * n)
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(NotingNode * n) 
{
	n->visitLeftChild(this);
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(LTingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(GTingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(EQingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(LTEingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(GTEingNode * n) 
{
	n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function adds declarations for the temporaries.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::addTemporaries()
{
	for (int i = 1; i <= lineDblTempMax; i++)
		*out << indent() << "double _dbltemp_" << i << " = 0;\n";

	for (int i = 1; i <= lineStrTempMax; i++)
		*out << indent() << "string _strtemp_" << i << " = \"\";\n";
}