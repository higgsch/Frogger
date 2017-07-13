//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents a visitor for generating output code
// that reflects the current AST.
// -----------------------------------------------------------------
#include "codeGenerationPhase.h"
#include "SubPhases\supportReqsSubPhase.h"
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

	localizeSymbolTable(UDF->symbols, rec);
	emitSymbolTable(UDF->symbols);

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

	for (int index = 0; index < prog->getNumberOfUDFs(); index++)
	{
		UDFRecord * currFunct = prog->getUDF(index);
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
	printSupportText(STATIC_SUPPORT::MAIN_DEC);
	printOpenBraceLine();

	printSupportText(INIT_STMT::ARGS);
	printSupportText(INIT_STMT::RANDOM);
	printSupportText(INIT_STMT::I_FILE);
	printSupportText(INIT_STMT::O_FILE);

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
	printSupportText(IMPORT_STMT::IO_STREAM);
	printSupportText(IMPORT_STMT::MATH);
	printSupportText(IMPORT_STMT::STD_LIB);
	printSupportText(IMPORT_STMT::TIME);
	printSupportText(IMPORT_STMT::F_STREAM);
	printSupportText(IMPORT_STMT::STRING);
	printSupportText(IMPORT_STMT::VECTOR);
	printSupportText(STATIC_SUPPORT::USING);
	printEmptyLine();
}

// ----------------------------------------------------------
// This function writes the given support text.
// @text: The support text to write.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printSupportText(SUPPORT_TEXT& text)
{
	if (text.isNeeded() && !text.isDefined)
	{
		printLine(text.getText());
		text.isDefined = true;
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
	printSupportText(VAR_DEF::ARGS);
	printSupportText(VAR_DEF::I_FILE);
	printSupportText(VAR_DEF::O_FILE);
	printSupportText(VAR_DEF::EMPTY_STRING);
	printEmptyLine();

	printSupportText(FUNCT_DEF::ROUND);
	printEmptyLine();
	printSupportText(FUNCT_DEF::RT);
	printEmptyLine();
}

// ----------------------------------------------------------
// This function adds declarations for each symbol in the 
// given symbol table.
// @symbols: The SymbolTable to emit.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::emitSymbolTable(SymbolTable * symbols)
{
	for (int i = 0; i < symbols->size(); i++)
	{
		Symbol* s = (*symbols)[i];

		if (!s->isLocal)
			continue;
		
		//emit variable declaration and initialization
		if (s->type == DT_DOUBLE)
			printLine(DATATYPE_TEXT::DOUBLE.getText() + " _" + s->id + " = 0;");
		else if (s->type == DT_STRING)
			printLine(DATATYPE_TEXT::STRING.getText() + " _" + s->id + " = \"\";");
		else
			printLine("Not_Defined _" + s->id + " = NULL;");
	}
}

// ----------------------------------------------------------
// This function marks symbols in the symbol table as local
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::localizeSymbolTable(SymbolTable *symbols, UDFRecord *currRec)
{
	for (int i = 0; i < symbols->size(); i++)
	{
		Symbol* s = (*symbols)[i];
		s->isLocal = true;

		if (s->id == SYMBOL_TEXT::ARGS.getText())
		{
			s->isLocal = false;
			continue;
		}

		for (int j = 0; j < currRec->args->size(); j++)
		{
			string argId = currRec->args->at(j)->name;
			if (s->id == argId)
			{
				s->isLocal = false;
				break;
			}
		}
	}
}

// ----------------------------------------------------------
// This function generates the built in functions.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInFunctions()
{
	printSupportText(FUNCT_DEF::TO_STRING);
	printSupportText(FUNCT_DEF::TO_ASCII);
	printSupportText(FUNCT_DEF::PARSE_DOUBLE);
	printSupportText(FUNCT_DEF::ASCII_AT);
	printSupportText(FUNCT_DEF::LENGTH);
	printSupportText(FUNCT_DEF::RETRIEVE_DOUBLE);
	printSupportText(FUNCT_DEF::RETRIEVE_STRING);
	printSupportText(FUNCT_DEF::RANDOM);
	printSupportText(FUNCT_DEF::READ);
	printSupportText(FUNCT_DEF::ELEMENT_AT);
	printSupportText(FUNCT_DEF::SIZE);
}

// ----------------------------------------------------------
// This function generates the built in commands.
//
// Version 4.2
// ----------------------------------------------------------
void CodeGenerationPhase::printBuiltInCommands()
{
	printSupportText(FUNCT_DEF::DISPLAY_DBL);
	printSupportText(FUNCT_DEF::DISPLAY_STR);
	printSupportText(FUNCT_DEF::OPEN_I);
	printSupportText(FUNCT_DEF::OPEN_O);
	printSupportText(FUNCT_DEF::WRITE);
	printSupportText(FUNCT_DEF::CLOSE_I);
	printSupportText(FUNCT_DEF::CLOSE_O);
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
		return DATATYPE_TEXT::DOUBLE.getText();
	case DT_STRING:
		return DATATYPE_TEXT::STRING.getText();
	case DT_NULL:
		return DATATYPE_TEXT::VOID.getText();
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

	if (id == SYMBOL_TEXT::ARGS.getText())
		printString(SYMBOL_TEXT::ARGS.getText());
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

		if (name == CommandTable::CMD_END_NULL->name)
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
	string pretext = (n->getDataType() == DT_STRING) ? 
		SYMBOL_TEXT::EMPTY_STRING.getText() + " + " : "";
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
	return name == FunctionTable::FUNCT_TO_STRING->name ||
		   name == FunctionTable::FUNCT_TO_ASCII->name ||
		   name == FunctionTable::FUNCT_PARSE_DOUBLE->name ||
		   name == FunctionTable::FUNCT_ASCII_AT->name ||
		   name == FunctionTable::FUNCT_LENGTH->name ||
		   name == FunctionTable::FUNCT_RETRIEVE_DOUBLE->name ||
		   name == FunctionTable::FUNCT_RETRIEVE_STRING->name ||
		   name == FunctionTable::FUNCT_RANDOM->name ||
		   name == FunctionTable::FUNCT_READ->name ||
		   name == FunctionTable::FUNCT_ELEMENT_AT->name ||
		   name == FunctionTable::FUNCT_SIZE->name;
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
	return name == CommandTable::CMD_END_NULL->name ||
		   name == CommandTable::CMD_DISPLAY_STR->name ||
		   name == CommandTable::CMD_OPEN_INPUT->name ||
		   name == CommandTable::CMD_OPEN_OUTPUT->name ||
		   name == CommandTable::CMD_WRITE->name ||
		   name == CommandTable::CMD_CLOSE_INPUT->name ||
		   name == CommandTable::CMD_CLOSE_OUTPUT->name;
}