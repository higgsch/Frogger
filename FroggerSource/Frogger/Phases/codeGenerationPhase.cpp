//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
#include "SubPhases\includesSubPhase.h"
#include "SubPhases\varDecSubPhase.h"
#include "SubPhases\tempAssignSubPhase.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 3.0
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase(SymbolTable* i_symbols)
{
	symbols = i_symbols;

	dblTempNo = 1; //temporaries are 1-indexed
	strTempNo = 1;
	
	indentDepth = 0;

	out = new ofstream();
}

// ----------------------------------------------------------
// This function initiates the phase over the AST.
// @n: The node representing the program.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ProgramNode * n)
{
	//emit the include statements code
	IncludesSubPhase* iSub = new IncludesSubPhase(out);
	n->accept(iSub);
	*out << "using namespace std;\n\n";

	if (iSub->needsString())
		*out << "const string emptyString = \"\";\n\n";

	if (iSub->needsRoundFunction())
		*out << "double round(double num) {\n"
			<< "\treturn (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);\n"
			<< "}\n\n";

	if (iSub->needsStringToDoubleFunction())
		*out << "double stringToDouble(string s) {\n"
			<< "\tif (isdigit(s[0]) || s[0] == '-')\n"
			<< "\t\treturn stod(s, NULL);\n"
			<< "\treturn 0;\n"
			<< "}\n\n";

	if (iSub->needsStringToAsciiFunction())
		*out << "double stringToAscii(string s, int loc) {\n"
			<< "\tif (loc < 0 || loc >= s.length())\n"
			<< "\t\treturn 0;\n"
			<< "\treturn s.at(loc);\n"
			<< "}\n\n";

	*out << "int main(int argc, char* argv[])\n{\n";

	if (iSub->hasRandomNode())
		*out << "\tsrand(time(NULL)); rand();\n";

	indentDepth++;

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth, symbols);
	n->accept(sub);
	sub->emitSymbolTable();
	sub->emitTemporaries();
	*out << endl << endl;

	 n->visitAllChildren(this);

	*out << "\n}" << endl; //close the c++ main function
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(JmpStmtNode * n)
{
	dblTempNo = 1; //restart temporary counters (1-indexed)
	strTempNo = 1;

	bool isOwnLine = (!n->isNested());

	if (isOwnLine) 
	{
		//emit this line's label
		*out << indent() << "__LABEL_" << n->getStmtNo() << ":" << endl;
		indentDepth++;
	}

	//generate temp assignments for the line
	n->visitThisStmt(new TempAssignSubPhase(out, indentDepth));

	//emit the line's code
	n->visitThisStmt(this);
	
	//emit this line's goto statement
	*out << indent() << "goto __LABEL_" << n->getJump() << ";" << endl;

	if (isOwnLine)
	{
		*out << endl;
		indentDepth--;
		n->visitNextStmt(this);
	}
}

// ----------------------------------------------------------
// This function processes an if statement.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IfNode * n)
{
	bool isOwnLine = (!n->isNested());

	if (isOwnLine)
	{
		*out << indent() << "__LABEL_" << n->getStmtNo() << ":" << endl;
		indentDepth++;
	}

	n->visitBoolExp(new TempAssignSubPhase(out, indentDepth));

	*out << indent() << "if (";
	n->visitBoolExp(this);
	*out << ")\n" << indent() << "{\n";
	indentDepth++;
	n->visitTrueStmt(this);
	indentDepth--;
	*out << indent() << "}\n";
	*out << indent() << "else\n" << indent() << "{\n";
	indentDepth++;
	n->visitFalseStmt(this);
	indentDepth--;
	*out << indent() << "}\n" << endl;

	if (isOwnLine)
		indentDepth--;

	n->visitNextStmt(this);
}

// ----------------------------------------------------------
// This function processes a variable reference.
// @n: The node representing the variable.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IdRefNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	//prepend identifiers to avoid c++ keyword conflicts
	*out << "_" << n->getLexeme();
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AssigningNode * n)
{
	*out << indent();
	n->visitAssignee(this);
	*out << " = (";
	n->visitAssignor(this);
	*out << ");" << endl;
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(FunctionCallNode * n)
{
	Function* funct = n->getFunct();
	if (funct->isUserDefined())
	{
		*out << "(";
		n->visitPrimary(this);
		*out << ")." << funct->name << "(";
		
		n->visitArgList(this);

		*out << ")";
	}
	else
	{
		string name = funct->name;

		if (name == "toString")
		{
			*out << "to_string(";
			n->visitPrimary(this);
			*out << ")";
			//<double>:toString() takes no arguments
		}
		else if (name == "toAscii")
		{
			*out << "(char) (";
			n->visitPrimary(this);
			*out << ")";
			//<double>:toAscii() takes no arguments
		}
		else if (name == "parseDouble")
		{
			*out << "stringToDouble(";
			n->visitPrimary(this);
			*out << ")";
			//<string>:parseDouble() takes no arguments
		}
		else if (name == "asciiAt")
		{
			*out << "stringToAscii(";
			n->visitPrimary(this);
			*out << ", ";
			n->visitArgList(this);
			*out << ")";
			//<string>:asciiAt(<double>) takes an argument
		}
		else if (name == "retrieveDouble")
		{
			*out << " _dbltemp_" << dblTempNo++ << " ";
		}
		else if (name == "retrieveString")
		{
			*out << " _strtemp_" << strTempNo++ << " ";
		}
		else if (name == "random")
		{
			*out << " ((double) rand() / (RAND_MAX)) ";
		}
		else
			this->semantic_error("Unrecognized function: " + name);
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(CommandCallNode * n)
{
	Command* cmd = n->getCmd();
	if (cmd->isUserDefined())
	{
		//*out << "(";
		//n->visitPrimary(this);
		//*out << ").";

		*out << cmd->name << "(";

		n->visitArgList(this);

		*out << ")";
	}
	else
	{
		string name = cmd->name;

		if (name == "end")
		{
			*out << indent() << "exit(0);" << endl;
		}
		else if (name == "display")
		{
			*out << indent() << "cout << (";
			n->visitArgList(this);
			*out << ");" << endl;
		}
		else
			this->semantic_error("Unrecognized command: " + name);
	}
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ArgListNode * n)
{
	n->visitThisArg(this);
	
	if (n->hasNextArg())
	{
		*out << ", ";
		n->visitNextArg(this);
	}
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DoubleConstingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << n->getLexeme();
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an addition operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AddingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	if (n->getDataType() == DT_STRING)
		*out << "emptyString + ";

	n->visitLeftOperand(this);
	*out << " + ";
	n->visitRightOperand(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(SubingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftOperand(this);
	*out << " - ";
	n->visitRightOperand(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(MulingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftOperand(this);
	*out << " * ";
	n->visitRightOperand(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftOperand(this);
	*out << " / ";
	n->visitRightOperand(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ModDivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "fmod( ";
	n->visitLeftOperand(this);
	*out << ", ";
	n->visitRightOperand(this);
	*out << " )";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IDivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "((int)round(";
	n->visitLeftOperand(this);
	*out << ")) / ((int)round(";
	n->visitRightOperand(this);
	*out << "))";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(RootingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "pow(";
	n->visitRightOperand(this);
	*out << ", 1.0 / ";
	n->visitLeftOperand(this);
	*out << ")";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ExpingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "pow(";
	n->visitLeftOperand(this);
	*out << ", ";
	n->visitRightOperand(this);
	*out << ")";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(NotingNode * n) 
{
	*out << "!( ";
	n->visitOperand(this);
	*out << " )";
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTingNode * n) 
{
	n->visitLeftOperand(this);
	*out << " < ";
	n->visitRightOperand(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTingNode * n) 
{
	n->visitLeftOperand(this);
	*out << " > ";
	n->visitRightOperand(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(EQingNode * n) 
{
	n->visitLeftOperand(this);
	*out << " == ";
	n->visitRightOperand(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTEingNode * n) 
{
	n->visitLeftOperand(this);
	*out << " <= ";
	n->visitRightOperand(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 3.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTEingNode * n) 
{
	n->visitLeftOperand(this);
	*out << " >= ";
	n->visitRightOperand(this);
}