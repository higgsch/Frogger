//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
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
// Version 4.0
// ----------------------------------------------------------
VarDecSubPhase::VarDecSubPhase(ostream* i_out, int i_tabCount, SymbolTable* i_symbols, UDFRecord * rec)
{
	currRec = rec;
	symbols = i_symbols;
	out = i_out;
	currStmtDblTempCount = 0;
	currStmtStrTempCount = 0;
	lineDblTempMax = 0;
	lineStrTempMax = 0;
	indentDepth = i_tabCount;
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
// Version 3.0
// ----------------------------------------------------------
void VarDecSubPhase::visit(IdRefNode * n)
{
	string id = n->getLexeme();

	Symbol s = Symbol(id, DT_NOT_DEFINED);

	if (!symbols->isDefined(&s))
	{
		//variable is not in SymbolTable
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
// This function adds declarations for each symbol in the 
// symbol table.
//
// Version 4.0
// ----------------------------------------------------------
void VarDecSubPhase::emitSymbolTable()
{
	for (int i = 0; i < symbols->size(); i++)
	{
		Symbol s = *(*symbols)[i];

		bool isDefinedInPrototype = false;
		for (int j = 0; j < currRec->args->size(); j++)
		{
			if (currRec->args->at(j)->name == s.id)
			{
				isDefinedInPrototype = true;
				break;
			}
		}

		if (isDefinedInPrototype)
			continue;
		
		//emit variable declaration and initialization
		if (s.type == DT_DOUBLE)
			*out << indent() << "double _" << s.id << " = 0;\n";
		else if (s.type == DT_STRING)
			*out << indent() << "string _" << s.id << " = \"\";\n";
		else if (s.type == DT_ARGS)
			continue;
		else
			*out << indent() << "Not_Defined _" << s.id << " = NULL;\n";
	}
}

// ----------------------------------------------------------
// This function adds declarations for the temporaries.
//
// Version 2.5
// ----------------------------------------------------------
void VarDecSubPhase::emitTemporaries()
{
	for (int i = 1; i <= lineDblTempMax; i++)
		*out << indent() << "double _dbltemp_" << i << " = 0;\n";

	for (int i = 1; i <= lineStrTempMax; i++)
		*out << indent() << "string _strtemp_" << i << " = \"\";\n";
}