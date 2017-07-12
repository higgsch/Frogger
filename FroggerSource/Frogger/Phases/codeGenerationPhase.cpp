//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
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
// This function generates the pre-program code.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printMetaCode(ProgramAST * progAST, ProgramStruct * progStruct)
{
	//emit the include statements code
	SupportReqsSubPhase * reqs = new SupportReqsSubPhase();
	reqs->gatherRequirements(progAST);
	delete reqs; reqs = NULL;

	emitUsingStatement();
	emitSupportCode();

	printBuiltInFunctions();
	printBuiltInCommands();

	printForwardDeclarations(progStruct);
	printMainFunction(progStruct->PEF->UDFName);
}

// ----------------------------------------------------------
// This function generates the code for a given UDF.
// @UDF: The UDF's struct.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printUDFCode(FunctionAST * UDF, UDFRecord * rec) 
{
	currUDFName = rec->UDFName;

	printLine(getFunctionPrototype(rec));
	printOpenBraceLine();

	//emit the variable declarations
	VarDecSubPhase * sub = new VarDecSubPhase(out, indentDepth, UDF->symbols, rec);
	UDF->root->accept(sub);
	sub->emitSymbolTable();
	delete sub; sub = NULL;

	printEmptyLine();
	printEmptyLine();

	UDF->root->accept(this);

	printEmptyLine();
	printCloseBraceLine();
	printEmptyLine();
}

// ----------------------------------------------------------
// This function generates the forward function declarations.
// @prog: The structure table for the program.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printForwardDeclarations(ProgramStruct * prog)
{
	printLine(getFunctionPrototype(prog->PEF) + ";");

	for (int index = 0; index < prog->UDFs->size(); index++)
	{
		UDFRecord * currFunct = (*(prog->UDFs))[index];
		printLine(getFunctionPrototype(currFunct) + ";");
	}

	printEmptyLine();
}

// ----------------------------------------------------------
// This function generates the code for main.
// @PEFName: The name of the PEF function.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printMainFunction(string PEFName)
{
	printLine("int main(int argc, char* argv[])");
	printOpenBraceLine();
	printLine("args = vector<string>(argv + 1, argv + argc);");

	if (FUNCT_DEF::RANDOM.isNeeded)
		printLine("srand(time(NULL)); rand();");

	if (VAR_DEF::I_FILE.isNeeded)
		printLine("in_file = ifstream();");

	if (VAR_DEF::O_FILE.isNeeded)
		printLine("out_file = ofstream();");

	printEmptyLine();
	printLine(PEFName + "();");
	printCloseBraceLine();
	printEmptyLine();
}

// ----------------------------------------------------------
// This function emits the using statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitUsingStatement()
{
	importIOStream();
	importMath();
	importStdLib();
	importTime();
	importFStream();

	*out << IMPORT_STMT::STRING.getText() << "\n"
		<< IMPORT_STMT::VECTOR.getText() << "\n"
		<< IMPORT_STMT::USING.getText() << "\n\n";
}

// ----------------------------------------------------------
// This function writes the include statment for the iostream
// library.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::importIOStream()
{
	if (IMPORT_STMT::IO_STREAM.isNeeded && !IMPORT_STMT::IO_STREAM.isDefined)
	{
		*out << IMPORT_STMT::IO_STREAM.getText() + "\n";
		IMPORT_STMT::IO_STREAM.isDefined = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the math
// library.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::importMath()
{
	if (IMPORT_STMT::MATH.isNeeded && !IMPORT_STMT::MATH.isDefined)
	{
		*out << IMPORT_STMT::MATH.getText() + "\n";
		IMPORT_STMT::MATH.isDefined = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the stdlib
// library.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::importStdLib()
{
	if (IMPORT_STMT::STD_LIB.isNeeded && !IMPORT_STMT::STD_LIB.isDefined)
	{
		*out << IMPORT_STMT::STD_LIB.getText() + "\n";
		IMPORT_STMT::STD_LIB.isDefined = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the time
// library.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::importTime()
{
	if (IMPORT_STMT::TIME.isNeeded && !IMPORT_STMT::TIME.isDefined)
	{
		*out << IMPORT_STMT::TIME.getText() + "\n";
		IMPORT_STMT::TIME.isDefined = true;
	}
}

// ----------------------------------------------------------
// This function writes the include statment for the fstream
// library.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::importFStream()
{
	if (IMPORT_STMT::F_STREAM.isNeeded && !IMPORT_STMT::F_STREAM.isDefined)
	{
		*out << IMPORT_STMT::F_STREAM.getText() + "\n";
		IMPORT_STMT::F_STREAM.isDefined = true;
	}
}

// ----------------------------------------------------------
// This function emits the global support code (constants and
// functions).
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitSupportCode()
{
	emitArgVector();
	emitFileStreams();
	emitEmptyString();

	emitRoundFunction();
	emitRtFunction();
}

// ----------------------------------------------------------
// This function emits the vector of argument strings.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitArgVector()
{
	*out << VAR_DEF::ARGS.getText() << "\n";
}

// ----------------------------------------------------------
// This function emits the in_file and out_file objects.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitFileStreams()
{
	if (VAR_DEF::I_FILE.isNeeded)
		*out << VAR_DEF::I_FILE.getText() << "\n";

	if (VAR_DEF::O_FILE.isNeeded)
		*out << VAR_DEF::O_FILE.getText() << "\n";
}

// ----------------------------------------------------------
// This function emits the emptyString constant.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitEmptyString()
{
	*out << VAR_DEF::EMPTY_STRING.getText() << "\n\n";
}

// ----------------------------------------------------------
// This function emits the round function.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitRoundFunction()
{
	if (FUNCT_DEF::ROUND.isNeeded)
		*out << FUNCT_DEF::ROUND.getText() << "\n\n";
}

// ----------------------------------------------------------
// This function emits the rt function.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitRtFunction()
{
	*out << FUNCT_DEF::RT.getText() << "\n\n";
}

// ----------------------------------------------------------
// This function generates the built in functions.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInFunctions()
{
	if (FUNCT_DEF::TO_STRING.isNeeded)
		printLine("string toString(double d) { return to_string(d); }");
	if (FUNCT_DEF::TO_ASCII.isNeeded)
		printLine("char toAscii(double d) { return (char) d; }");
	if (FUNCT_DEF::PARSE_DOUBLE.isNeeded)
	{
		printLine("double parseDouble(string s)"); 
		printOpenBraceLine();
		printLine("if (isdigit(s[0]) || s[0] == '-')");
		printOpenBraceLine();
		printLine("return stod(s, NULL);"); 
		printCloseBraceLine();
		printLine("return 0;");
		printCloseBraceLine();
	}
	if (FUNCT_DEF::ASCII_AT.isNeeded)
	{
		printLine("double asciiAt(string s, int loc)");
		printOpenBraceLine();
		printLine("if (loc < 0 || loc >= s.length())");
		printOpenBraceLine();
		printLine("return 0;");
		printCloseBraceLine();
		printLine("return s.at(loc);");
		printCloseBraceLine();
	}
	if (FUNCT_DEF::LENGTH.isNeeded)
		printLine("double length(string s) { return (emptyString + s).size(); }");
	if (FUNCT_DEF::RETRIEVE_DOUBLE.isNeeded)
	{
		printLine("double retrieveDouble()");
		printOpenBraceLine();
		printLine("double d = 0;");
		printLine("cin >> d;");
		printLine("return d;");
		printCloseBraceLine();
	}
	if (FUNCT_DEF::RETRIEVE_STRING.isNeeded)
	{
		printLine("string retrieveString()");
		printOpenBraceLine();
		printLine("string s = "";");
		printLine("cin >> s;");
		printLine("return s;");
		printCloseBraceLine();
	}
	if (FUNCT_DEF::RANDOM.isNeeded)
		printLine("double random() { return ((double) rand() / (RAND_MAX)); }");
	if (FUNCT_DEF::READ.isNeeded)
		printLine("char read() { return (char)(in_file.get()); }");
	if (FUNCT_DEF::ELEMENT_AT.isNeeded)
	{
		printLine("string elementAt(vector<string> v, int index)");
		printOpenBraceLine();
		printLine("if (index < 0 || index >= v.size())");
		printOpenBraceLine();
		printLine("return \"\";");
		printCloseBraceLine();
		printLine("return v[index];");
		printCloseBraceLine();
	}
	if (FUNCT_DEF::SIZE.isNeeded)
		printLine("double size(vector<string> v) { return v.size(); }");
}

// ----------------------------------------------------------
// This function generates the built in commands.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInCommands()
{
	if (FUNCT_DEF::DISPLAY.isNeeded)
	{
		printLine("void display(double d) { cout << d; }");
		printLine("void display(string s) { cout << s; }");
	}
	if (FUNCT_DEF::OPEN_I.isNeeded)
		printLine("void openI(string s) { in_file.open(s); }");
	if (FUNCT_DEF::OPEN_O.isNeeded)
		printLine("void openO(string s) { out_file.open(s); }");
	if (FUNCT_DEF::WRITE.isNeeded)
		printLine("void write(string s) { out_file << s; }");
	if (FUNCT_DEF::CLOSE_I.isNeeded)
		printLine("void closeI() { in_file.close(); }");
	if (FUNCT_DEF::CLOSE_O.isNeeded)
		printLine("void closeO() { out_file.close(); }");
}

// ----------------------------------------------------------
// This function generates the function prototype of the given
// UDFRecord.
// @rec: The function to print.
//
// Version 4.2
// ----------------------------------------------------------
string CodeGenerationPhase::getFunctionPrototype(UDFRecord * rec)
{
	return typeString(rec->returnType) + " " + rec->UDFName + "(" + argsString(rec->args) + ")";
}

// ----------------------------------------------------------
// This function generates the label text for the given index
// in the current UDF.
// @labelIndex: The index of the label to print.
//
// Version 4.2
// ----------------------------------------------------------
string CodeGenerationPhase::getLabelText(int labelIndex)
{
	return "__LABEL_" + currUDFName + "_" + to_string(labelIndex);
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
		printString("(");

	printString(pretext);
	n->visitLeftOperand(this);
	printString(midtext);
	n->visitRightOperand(this);
	printString(posttext);
	
	if (n->getParenNesting() > 0)
		printString(")");
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
		printLine(getLabelText(n->getStmtNo()) + ":");
		indentDepth++;
	}

	//emit the line's code
	n->visitThisStmt(this);
	
	//emit this line's goto statement
	printLine("goto " + getLabelText(n->getJump()) + ";");

	if (isOwnLine)
	{
		printEmptyLine();
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
		//emit this line's label
		printLine(getLabelText(n->getStmtNo()) + ":");
		indentDepth++;
	}

	printIndent(); printString("if ("); n->visitBoolExp(this); printString(")\n");
	printOpenBraceLine();

	n->visitTrueStmt(this);

	printCloseBraceLine();
	printLine("else");
	printOpenBraceLine();

	n->visitFalseStmt(this);

	printCloseBraceLine();

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
		printString("(");

	string id = n->getLexeme();

	if (id == "args")
		printString("args");
	else
		//prepend identifiers to avoid c++ keyword conflicts
		printString("_" + n->getLexeme());
	
	if (n->getParenNesting() > 0)
		printString(")");
}

// ----------------------------------------------------------
// This function processes an assignment statement.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(AssigningNode * n)
{
	printIndent();
	n->visitAssignee(this);
	printString(" = (");
	n->visitAssignor(this);
	printString(");\n");
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
		if (!validBuiltInFunctionName(name))
			semantic_error("Unrecognized function: " + name, n->getLineNo());
	}

	/* For User Defined
	if (funct->parentType != DT_NULL)
	{
		printString("(");
		n->visitPrimary(this);
		printString(").");
	}*/

	printString(name + "(");

	if (n->getPrimary() == NULL)
		n->visitArgList(this);
	else
	{
		n->visitPrimary(this);
		if (n->getArgListLength() > 0)
		{
			printString(", ");
			n->visitArgList(this);
		}
	}
	printString(")");
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
		if (!validBuiltInCommandName(name))
			semantic_error("Unrecognized command: " + name, n->getLineNo());

		if (name == "end")
		{
			printIndent(); printString("return ");
			n->visitArgList(this);
			printString(";\n");
			return;
		}
	}

	//printString("(");
	//n->visitPrimary(this);
	//printString(").");

	printIndent(); printString(name + "(");
	n->visitArgList(this);
	printString(");\n");
}

// ----------------------------------------------------------
// This function processes an element in an argument list.
// @n: The node representing the statement.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(ArgListNode * n)
{
	n->visitThisArg(this);
	
	if (n->hasNextArg())
	{
		printString(", ");
		n->visitNextArg(this);
	}
}

// ----------------------------------------------------------
// This function processes a double literal.
// @n: The node representing the literal.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(DoubleConstingNode * n)
{
	if (n->getParenNesting() > 0)
		printString("(");

	printString(n->getLexeme());
	
	if (n->getParenNesting() > 0)
		printString(")");
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
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::visit(NotingNode * n) 
{
	printString("!( ");
	n->visitOperand(this);
	printString(" )");
}

// ----------------------------------------------------------
// This function determines if the given name is a valid
// built-in function name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::validBuiltInFunctionName(string name)
{
	return name == "toString" ||
		   name == "toAscii" ||
		   name == "parseDouble" ||
		   name == "asciiAt" ||
		   name == "length" ||
		   name == "retrieveDouble" ||
		   name == "retrieveString" ||
		   name == "random" ||
		   name == "read" ||
		   name == "elementAt" ||
		   name == "size";
}

// ----------------------------------------------------------
// This function determines if the given name is a valid
// built-in command name.
// @name: The name to test.
//
// Version 4.2
// ----------------------------------------------------------
bool CodeGenerationPhase::validBuiltInCommandName(string name)
{
	return name == "end" ||
		   name == "display" ||
		   name == "openI" ||
		   name == "openO" ||
		   name == "write" ||
		   name == "closeI" ||
		   name == "closeO";
}