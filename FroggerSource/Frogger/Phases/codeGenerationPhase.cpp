//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
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
// Version 4.0
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase()
{
	dblTempNo = 1; //temporaries are 1-indexed
	strTempNo = 1;
	
	indentDepth = 0;

	out = new ofstream();

	needsRand = false;
	needsInFile = false;
	needsOutFile = false;

	currUDFName = "<META>";
}

// ----------------------------------------------------------
// This function generates the pre-function code.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::printMetaCode(ProgramAST * progAST, ProgramStruct * progStruct)
{
	//emit the include statements code
	IncludesSubPhase* iSub = new IncludesSubPhase(out);
	progAST->PEF->root->accept(iSub);

	int UDFCount = progAST->UDFs->size();
	if (UDFCount > 0)
	{
		int index = 0;
		while (index < UDFCount)
		{
			(*(progAST->UDFs))[index]->root->accept(iSub);
			index++;
		}
	}

	iSub->emitUsingStatment();
	iSub->emitSupportCode();
	needsRand = iSub->hasRandomNode();
	needsInFile = iSub->needsInputFile();
	needsOutFile = iSub->needsOutputFile();
	delete iSub; iSub = NULL;

	printForwardDeclarations(progStruct);

	*out << "int main(int argc, char* argv[])\n{\n"
	<< "\targs = vector<string>(argv + 1, argv + argc);\n";

	if (needsRand)
		*out << "\tsrand(time(NULL)); rand();\n";

	if (needsInFile)
		*out << "\tin_file = ifstream();\n";

	if (needsOutFile)
		*out << "\tout_file = ofstream();\n";

	*out << "\n";
	
	*out << "\t" << progStruct->PEF->UDFName << "();\n"
		<< "}\n\n";
}

// ----------------------------------------------------------
// This function generates the code for the PEF.
// @PEF: The PEF's struct.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::printPEFCode(FunctionAST * PEF, UDFRecord * rec)
{
	printUDFCode(PEF, rec);
}

// ----------------------------------------------------------
// This function generates the code for a given UDF.
// @UDF: The UDF's struct.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::printUDFCode(FunctionAST * UDF, UDFRecord * rec) 
{
	currUDFName = rec->UDFName;

	printFunctionPrototype(rec);
	*out << " \n" 
		<< "{\n";
	indentDepth++;

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth, UDF->symbols);
	UDF->root->accept(sub);
	sub->emitSymbolTable();
	sub->emitTemporaries();
	delete sub; sub = NULL;

	*out << endl << endl;

	UDF->root->accept(this);

	*out << "\n}" << endl;
}

// ----------------------------------------------------------
// This function generates the forward function declarations.
// @prog: The structure table for the program.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::printForwardDeclarations(ProgramStruct * prog)
{
	printFunctionPrototype(prog->PEF);
	*out << ";\n";

	int index = 0;
	while (index < prog->UDFs->size())
	{
		printFunctionPrototype((*(prog->UDFs))[index]);
		*out << ";\n";

		index++;
	}

	*out << endl;
}

// ----------------------------------------------------------
// This function generates the function prototype of the given
// UDFRecord.
// @rec: The function to print.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::printFunctionPrototype(UDFRecord * rec)
{
	*out << typeString(rec->returnType) << " " << rec->UDFName << "(" << argsString(rec->args) << ")";
}

// ----------------------------------------------------------
// This function returns the output string corresponding to
// the given DataType.
// @dt: The DataType to print.
//
// Version 4.0
// ----------------------------------------------------------
string CodeGenerationPhase::typeString(DataType dt)
{
	switch (dt)
	{
	case DT_DOUBLE:
		return "double";
	case DT_STRING:
		return "string";
	case DT_NULL:
		return "void";
	default:
		return "UNDEFINED TYPE";
	}
}

// ----------------------------------------------------------
// This function returns a string representation of the
// argument list.
// @args: The argument list to convert.
//
// Version 4.0
// ----------------------------------------------------------
string CodeGenerationPhase::argsString(vector<argPair *> * args)
{
	string result = "";
	int index = 0;

	if (args->size() > 0)
	{
		result = typeString((*args)[index]->type) + " _" + (*args)[index]->name;
		index++;
	}

	while (index < args->size())
	{
		result = result + ", " + typeString((*args)[index]->type) + " _" + (*args)[index]->name;
		index++;
	}

	return result;
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(JmpStmtNode * n)
{
	dblTempNo = 1; //restart temporary counters (1-indexed)
	strTempNo = 1;

	bool isOwnLine = (!n->isNested());

	if (isOwnLine) 
	{
		//emit this line's label
		*out << indent() << "__LABEL_" << currUDFName << "_" << n->getStmtNo() << ":" << endl;
		indentDepth++;
	}

	//generate temp assignments for the line
	n->visitThisStmt(new TempAssignSubPhase(out, indentDepth));

	//emit the line's code
	n->visitThisStmt(this);
	
	//emit this line's goto statement
	*out << indent() << "goto __LABEL_" << currUDFName << "_" << n->getJump() << ";" << endl;

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
// Version 4.0
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IfNode * n)
{
	bool isOwnLine = (!n->isNested());

	if (isOwnLine)
	{
		*out << indent() << "__LABEL_" << currUDFName << "_" << n->getStmtNo() << ":" << endl;
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
// Version 3.3
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
		else if (name == "length")
		{
			*out << "(emptyString + ";
			n->visitPrimary(this);
			*out << ").size()";
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
		else if (name == "read")
		{
			*out << "(char)(in_file.get())";
		}
		else if (name == "elementAt")
		{
			*out << "elemAt(args,";
			n->visitArgList(this);
			*out << ")";
		}
		else if (name == "size")
		{
			*out << "args.size()";
		}
		else
			this->semantic_error("Unrecognized function: " + name, n->getLineNo());
	}
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 3.2
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
		else if (name == "openI")
		{
			*out << indent() << "in_file.open(";
			n->visitArgList(this);
			*out << ");\n";
		}
		else if (name == "openO")
		{
			*out << indent() << "out_file.open(";
			n->visitArgList(this);
			*out << ");\n";
		}
		else if (name == "write")
		{
			*out << indent() << "out_file << (";
			n->visitArgList(this);
			*out << ");\n";
		}
		else if (name == "closeI")
		{
			*out << indent() << "in_file.close();\n";
		}
		else if (name == "closeO")
		{
			*out << indent() << "out_file.close();\n";
		}
		else
			this->semantic_error("Unrecognized command: " + name, n->getLineNo());
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