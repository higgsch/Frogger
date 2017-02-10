//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.4
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
// This constructor generates a CodeGenerationPhase for the
// given output stream and AST.
// @outstream: The output stream to print to.
// @root: The root node for the AST.
//
// Version 2.2
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase(ostream* outstream)
{
	out = outstream;
	tempNo = 1; //temporaries are 1-indexed
	indentDepth = 0;
}

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
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth);
	n->accept(sub);
	sub->addTemporaries();
	*out << endl << endl;

	 n->visitAllChildren(this);
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(JmpStmtNode * n)
{
	tempNo = 1; //restart temporary counter (1-indexed)

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
// Version 2.0
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
// This function processes a retrieve statement.
// @n: The node representing the retrieve statement.
//
// Version 1.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(RetrievalNode * n)
{
	*out << " _temp_" << tempNo++ << " ";
}

// ----------------------------------------------------------
// This function processes a display statement.
// @n: The node representing the display statement.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DisplayingNode * n)
{
	*out << indent() << "cout << (";
	n->visitLeftChild(this);
	*out << ");" << endl;
}

// ----------------------------------------------------------
// This function processes a random statement.
// @n: The node representing the random statement.
//
// Version 2.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(RandomingNode * n)
{
	*out << " ((double) rand() / (RAND_MAX)) ";
}

// ----------------------------------------------------------
// This function processes an end statement.
// @n: The node representing the statement.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(EndingNode * n)
{
	*out << indent() << "exit(0);" << endl;
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
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AssigningNode * n)
{
	*out << indent();
	n->visitLeftChild(this);
	*out << " = (";
	n->visitRightChild(this);
	*out << ");" << endl;
}

// ----------------------------------------------------------
// This function processes a function call.
// @n: The node representing the statement.
//
// Version 2.4
// ----------------------------------------------------------
void CodeGenerationPhase::visit(FunctionCallNode * n)
{
	Function* funct = n->getFunct();
	if (funct->isUserFunction())
	{
		*out << "(";
		n->visitLeftChild(this);
		*out << ")." << funct->name << "(";

		if (n->getRightChild() != NULL)
			n->visitRightChild(this);

		*out << ")";
	}
	else
	{
		string name = funct->name;

		if (name == "toString")
		{
			*out << "to_string(";
			n->visitLeftChild(this);
			*out << ")";
			//<double>:toString() takes no arguments
		}
		else if (name == "toAscii")
		{
			*out << "(char) (";
			n->visitLeftChild(this);
			*out << ")";
			//<double>:toAscii() takes no arguments
		}
		else if (name == "parseDouble")
		{
			*out << "stringToDouble(";
			n->visitLeftChild(this);
			*out << ")";
			//<string>:parseDouble() takes no arguments
		}
		else if (name == "asciiAt")
		{
			*out << "stringToAscii(";
			n->visitLeftChild(this);
			*out << ", ";
			n->visitRightChild(this);
			*out << ")";
			//<string>:asciiAt(<double>) takes an argument
		}
		else
			this->semantic_error("Unrecognized function: " + name);
	}
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 2.4
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ArgListNode * n)
{
	n->visitLeftChild(this);
	
	if (n->getRightChild() != NULL)
	{
		*out << ", ";
		n->visitRightChild(this);
	}
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(StringConstingNode * n)
{
	*out << n->getLexeme();
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
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AddingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	if (n->getDataType() == DT_STRING)
		*out << "emptyString + ";

	n->visitLeftChild(this);
	*out << " + ";
	n->visitRightChild(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a subtraction operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(SubingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftChild(this);
	*out << " - ";
	n->visitRightChild(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a multiplication operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(MulingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftChild(this);
	*out << " * ";
	n->visitRightChild(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a division operation.
// @n: The node representing the operation.
//
// Version 1.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	n->visitLeftChild(this);
	*out << " / ";
	n->visitRightChild(this);
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a modulus division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ModDivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "fmod( ";
	n->visitLeftChild(this);
	*out << ", ";
	n->visitRightChild(this);
	*out << " )";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an integer division operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IDivingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "((int)round(";
	n->visitLeftChild(this);
	*out << ")) / ((int)round(";
	n->visitRightChild(this);
	*out << "))";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a rootation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(RootingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "pow(";
	n->visitRightChild(this);
	*out << ", 1.0 / ";
	n->visitLeftChild(this);
	*out << ")";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes an exponentiation operation.
// @n: The node representing the operation.
//
// Version 2.1
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ExpingNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << "pow(";
	n->visitLeftChild(this);
	*out << ", ";
	n->visitRightChild(this);
	*out << ")";
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a not operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(NotingNode * n) 
{
	*out << "!( ";
	n->visitLeftChild(this);
	*out << " )";
}

// ----------------------------------------------------------
// This function processes a less than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTingNode * n) 
{
	n->visitLeftChild(this);
	*out << " < ";
	n->visitRightChild(this);
}

// ----------------------------------------------------------
// This function processes a greater than comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTingNode * n) 
{
	n->visitLeftChild(this);
	*out << " > ";
	n->visitRightChild(this);
}

// ----------------------------------------------------------
// This function processes an equivalence comparison operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(EQingNode * n) 
{
	n->visitLeftChild(this);
	*out << " == ";
	n->visitRightChild(this);
}

// ----------------------------------------------------------
// This function processes a less than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(LTEingNode * n) 
{
	n->visitLeftChild(this);
	*out << " <= ";
	n->visitRightChild(this);
}

// ----------------------------------------------------------
// This function processes a greater than or equal comparison 
// operation.
// @n: The node representing the operation.
//
// Version 2.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(GTEingNode * n) 
{
	n->visitLeftChild(this);
	*out << " >= ";
	n->visitRightChild(this);
}