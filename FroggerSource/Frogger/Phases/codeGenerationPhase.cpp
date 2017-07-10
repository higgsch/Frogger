//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
#include "SubPhases\includesSubPhase.h"
#include "SubPhases\varDecSubPhase.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor.
//
// Version 4.2
// ----------------------------------------------------------
CodeGenerationPhase::CodeGenerationPhase()
{	
	out = new ofstream();
	indentDepth = 0;
	currUDFName = "<META>";
}

// ----------------------------------------------------------
// This function generates the pre-function code.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printMetaCode(ProgramAST * progAST, ProgramStruct * progStruct)
{
	//emit the include statements code
	IncludesSubPhase iSub = IncludesSubPhase(out);
	iSub.emitIncludesStatements(progAST);
	iSub.emitSupportCode();

	printBuiltInFunctions(&iSub);
	printBuiltInCommands(&iSub);

	printForwardDeclarations(progStruct);
	printMainFunction(progStruct->PEF->UDFName, &iSub);
}

// ----------------------------------------------------------
// This function generates the code for a given UDF.
// @UDF: The UDF's struct.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printUDFCode(FunctionAST * UDF, UDFRecord * rec) 
{
	indentDepth = 0; 
	currUDFName = rec->UDFName;

	printFunctionPrototype(rec);
	*out << " \n" 
		<< "{\n";
	indentDepth++;

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth, UDF->symbols, rec);
	UDF->root->accept(sub);
	sub->emitSymbolTable();
	delete sub; sub = NULL;

	*out << endl << endl;

	UDF->root->accept(this);

	*out << "\n}\n" << endl;
}

// ----------------------------------------------------------
// This function generates the forward function declarations.
// @prog: The structure table for the program.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printForwardDeclarations(ProgramStruct * prog)
{
	printFunctionPrototype(prog->PEF);
	*out << ";\n";

	for (int index = 0; index < prog->UDFs->size(); index++)
	{
		printFunctionPrototype((*(prog->UDFs))[index]);
		*out << ";\n";
	}

	*out << endl;
}

// ----------------------------------------------------------
// This function generates the code for main.
// @PEFName: The name of the PEF function.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printMainFunction(string PEFName, IncludesSubPhase * iSub)
{
	*out << "int main(int argc, char* argv[])\n{\n"
	<< "\targs = vector<string>(argv + 1, argv + argc);\n";

	if (iSub->hasRandomNode())
		*out << "\tsrand(time(NULL)); rand();\n";

	if (iSub->needsInputFile())
		*out << "\tin_file = ifstream();\n";

	if (iSub->needsOutputFile())
		*out << "\tout_file = ofstream();\n";

	*out << "\n";
	
	*out << "\t" << PEFName << "();\n"
		<< "}\n\n";
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
// This function generates the label text for the given index
// in the current UDF.
// @labelIndex: The index of the label to print.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printLabelText(int labelIndex)
{
	*out << "__LABEL_" << currUDFName << "_" << labelIndex;
}

// ----------------------------------------------------------
// This function generates the built in functions.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInFunctions(IncludesSubPhase * iSub)
{
	if (iSub->needsToStringFunction())
		*out << "string toString(double d) { return to_string(d); }\n";
	if (iSub->needsToAsciiFunction())
		*out << "char toAscii(double d) { return (char) d; }\n";
	if (iSub->needsParseDoubleFunction())
		*out << "double parseDouble(string s) {\n"
				<< "\tif (isdigit(s[0]) || s[0] == '-')\n"
				<< "\t\treturn stod(s, NULL);\n"
				<< "\treturn 0;\n"
			<< "}\n";
	if (iSub->needsAsciiAtFunction())
		*out << "double asciiAt(string s, int loc) {\n"
				<< "\tif (loc < 0 || loc >= s.length())\n"
				<< "\t\treturn 0;\n"
				<< "\treturn s.at(loc);\n"
			<< "}\n";
	if (iSub->needsLengthFunction())
		*out << "double length(string s) { return (emptyString + s).size(); }\n";
	if (iSub->needsRetrieveDoubleFunction())
		*out << "double retrieveDouble() {\n"
				<< "\tdouble d = 0;\n"
				<< "\tcin >> d;\n"
				<< "\treturn d;\n"
			<< "}\n";
	if (iSub->needsRetrieveStringFunction())
		*out << "string retrieveString() {\n"
				<< "\tstring s = "";\n"
				<< "\tcin >> s;\n"
				<< "\treturn s;\n"
			<< "}\n";
	if (iSub->needsRandomFunction())
		*out << "double random() { return ((double) rand() / (RAND_MAX)); }\n";
	if (iSub->needsReadFunction())
		*out << "char read() { return (char)(in_file.get()); }\n";
	if (iSub->needsElementAtFunction())
		*out << "string elementAt(vector<string> v, int index) {\n"
				<< "\tif (index < 0 || index >= v.size())\n"
				<< "\t\treturn \"\";\n"
				<< "\treturn v[index];\n"
			<< "}\n";
	if (iSub->needsSizeFunction())
		*out << "double size(vector<string> v) { return v.size(); }\n";
}

// ----------------------------------------------------------
// This function generates the built in commands.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInCommands(IncludesSubPhase * iSub)
{
	if (iSub->needsDisplayCommand())
	{
		*out << "void display(double d) { cout << d; }\n";
		*out << "void display(string s) { cout << s; }\n";
	}
	if (iSub->needsOpenICommand())
		*out << "void openI(string s) { in_file.open(s); }\n";
	if (iSub->needsOpenOCommand())
		*out << "void openO(string s) { out_file.open(s); }\n";
	if (iSub->needsWriteCommand())
		*out << "void write(string s) { out_file << s; }\n";
	if (iSub->needsCloseICommand())
		*out << "void closeI() { in_file.close(); }\n";
	if (iSub->needsCloseOCommand())
		*out << "void closeO() { out_file.close(); }\n";
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
// Version 4.2
// ----------------------------------------------------------
string CodeGenerationPhase::argsString(vector<argPair *> * args)
{
	string result = "";
	int index = 0;

	while (index < args->size())
	{
		argPair * arg = (*args)[index];
		result = result + typeString(arg->type) + " _" + arg->name;
		index++;

		if (index != args->size())
			result = result + ", ";
	}

	return result;
}

// ----------------------------------------------------------
// This function emits the code for a BinaryOpNode.
// @n: The BinaryOpNode to process.
// @pretext: The text to emit before the left operand.
// @midtext: The text to emit between left and right operands.
// @posttext: The text to emit after the right operand.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::processBinaryOpNode(BinaryOpNode * n, string pretext, string midtext, string posttext)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	*out << pretext;
	n->visitLeftOperand(this);
	*out << midtext;
	n->visitRightOperand(this);
	*out << posttext;
	
	if (n->getParenNesting() > 0)
		*out << ")";
}

// ----------------------------------------------------------
// This function processes a line of code.
// @n: The node representing the line.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(JmpStmtNode * n)
{
	bool isOwnLine = (!n->isNested());

	if (isOwnLine) 
	{
		//emit this line's label
		*out << indent();
		printLabelText(n->getStmtNo());
		*out << ":" << endl; 

		indentDepth++;
	}

	//emit the line's code
	n->visitThisStmt(this);
	
	//emit this line's goto statement
	*out << indent() << "goto ";
	printLabelText(n->getJump());
	*out << ";" << endl; 

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
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IfNode * n)
{
	bool isOwnLine = (!n->isNested());

	if (isOwnLine)
	{
		*out << indent();
		printLabelText(n->getStmtNo());
		*out << ":" << endl;
		indentDepth++;
	}

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
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(IdRefNode * n)
{
	if (n->getParenNesting() > 0)
		*out << "(";

	string id = n->getLexeme();

	if (id == "args")
		*out << "args";
	else
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
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(FunctionCallNode * n)
{
	Function* funct = n->getFunct();
	string name = funct->name;

	if (!(funct->isUserDefined()))
	{
		if (invalidBuiltInFunctionName(name))
			semantic_error("Unrecognized function: " + name, n->getLineNo());
	}

	/* For User Defined
	if (funct->parentType != DT_NULL)
	{
		*out << "(";
		n->visitPrimary(this);
		*out << ").";
	}*/

	*out << name << "(";

	if (n->getPrimary() == NULL)
		n->visitArgList(this);
	else
	{
		n->visitPrimary(this);
		if (n->getArgListLength() > 0)
		{
			*out << ", ";
			n->visitArgList(this);
		}
	}
	*out << ")";
}

// ----------------------------------------------------------
// This function processes a command call.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(CommandCallNode * n)
{
	Command* cmd = n->getCmd();
	string name = cmd->name;

	if (!cmd->isUserDefined())
	{
		if (invalidBuiltInCommandName(name))
			semantic_error("Unrecognized command: " + name, n->getLineNo());

		if (name == "end")
		{
			*out << indent() << "return "; 
			n->visitArgList(this);
			*out << ";" << endl;
			return;
		}
	}

	//*out << "(";
	//n->visitPrimary(this);
	//*out << ").";

	*out << indent() << name << "(";
	n->visitArgList(this);
	*out << ");\n";
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
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AddingNode * n)
{
	string pretext = (n->getDataType() == DT_STRING) ? "emptyString + " : "";
	processBinaryOpNode(n, pretext, " + ", "");
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
// This function determines if the given name is a valid
// built-in function name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::invalidBuiltInFunctionName(string name)
{
	return name != "toString" &&
		   name != "toAscii" &&
		   name != "parseDouble" &&
		   name != "asciiAt" &&
		   name != "length" &&
		   name != "retrieveDouble" &&
		   name != "retrieveString" &&
		   name != "random" &&
		   name != "read" &&
		   name != "elementAt" &&
		   name != "size";
}

// ----------------------------------------------------------
// This function determines if the given name is a valid
// built-in command name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::invalidBuiltInCommandName(string name)
{
	return name != "end" &&
		   name != "display" &&
		   name != "openI" &&
		   name != "openO" &&
		   name != "write" &&
		   name != "closeI" &&
		   name != "closeO";
}