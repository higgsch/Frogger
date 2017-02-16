//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
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
// Version 3.0
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
// Version 3.0
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