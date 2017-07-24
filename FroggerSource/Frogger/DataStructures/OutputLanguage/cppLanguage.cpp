//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents the CPPLanguage package.
// -----------------------------------------------------------------
#include "cppLanguage.h"
using namespace std;

const string CPPLanguage::OPEN_BRACE = "{";
const string CPPLanguage::CLOSE_BRACE = "}";
const string CPPLanguage::OPEN_NEST = "(";
const string CPPLanguage::CLOSE_NEST = ")";
const string CPPLanguage::INDENT = "\t";

// ----------------------------------------------------------
// This function initializes the language elements.
//
// Version 4.2
// ----------------------------------------------------------
void CPPLanguage::initOutputTexts()
{
	DT_DOUBLE = OUTPUT_TEXT("double");
	DT_STRING = OUTPUT_TEXT("string");
	DT_VOID = OUTPUT_TEXT("void");

	SYM_ARGS = OUTPUT_TEXT("args");
	SYM_I_FILE = OUTPUT_TEXT("in_file");
	SYM_O_FILE = OUTPUT_TEXT("out_file");
	SYM_EMPTY_STRING = OUTPUT_TEXT("emptyString");

	FUNCTNAME_TO_STRING = OUTPUT_TEXT("toString");
	FUNCTNAME_TO_ASCII = OUTPUT_TEXT("toAscii");
	FUNCTNAME_PARSE_DOUBLE = OUTPUT_TEXT("parseDouble");
	FUNCTNAME_ASCII_AT = OUTPUT_TEXT("asciiAt");
	FUNCTNAME_LENGTH = OUTPUT_TEXT("length");
	FUNCTNAME_RETRIEVE_DOUBLE = OUTPUT_TEXT("retrieveDouble");
	FUNCTNAME_RANDOM = OUTPUT_TEXT("random");
	FUNCTNAME_RETRIEVE_STRING = OUTPUT_TEXT("retrieveString");
	FUNCTNAME_READ = OUTPUT_TEXT("read");
	FUNCTNAME_ELEMENT_AT = OUTPUT_TEXT("elementAt");
	FUNCTNAME_SIZE = OUTPUT_TEXT("size");

	CMDNAME_END_NULL = OUTPUT_TEXT("end");
	CMDNAME_END_STR = OUTPUT_TEXT("end");
	CMDNAME_END_DBL = OUTPUT_TEXT("end");
	CMDNAME_DISPLAY_STR = OUTPUT_TEXT("display");
	CMDNAME_DISPLAY_DBL = OUTPUT_TEXT("display");
	CMDNAME_OPEN_INPUT = OUTPUT_TEXT("openI");
	CMDNAME_CLOSE_INPUT = OUTPUT_TEXT("closeI");
	CMDNAME_WRITE = OUTPUT_TEXT("write");
	CMDNAME_OPEN_OUTPUT = OUTPUT_TEXT("openO");
	CMDNAME_CLOSE_OUTPUT = OUTPUT_TEXT("closeO");

	IMP_IO_STREAM = IMPORT_STMT("#include <iostream>");
	IMP_MATH = IMPORT_STMT("#include <math.h>");
	IMP_STD_LIB = IMPORT_STMT("#include <stdlib.h>");
	IMP_TIME = IMPORT_STMT("#include <time.h>");
	IMP_F_STREAM = IMPORT_STMT("#include <fstream>");
	IMP_STRING = IMPORT_STMT("#include <string>");
	IMP_VECTOR = IMPORT_STMT("#include <vector>");

	SYMDEF_ARGS = VAR_DEF("vector<string> " + SYM_ARGS + ";");
	SYMDEF_I_FILE = VAR_DEF("ifstream " + SYM_I_FILE + ";");
	SYMDEF_O_FILE = VAR_DEF("ofstream " + SYM_O_FILE + ";");
	SYMDEF_EMPTY_STRING = VAR_DEF("const string " + SYM_EMPTY_STRING + " = \"\";");

	FUNCTDEF_ROUND = FUNCT_DEF("double round(double num) {\n\treturn (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);\n}");
	FUNCTDEF_RT = FUNCT_DEF("double rt(double l, double r) {\n\t return pow(r, 1.0 / l);\n}");
	FUNCTDEF_TO_STRING = FUNCT_DEF("string " + FUNCTNAME_TO_STRING + "(double d) { return to_string(d); }");
	FUNCTDEF_TO_ASCII = FUNCT_DEF("char " + FUNCTNAME_TO_ASCII + "(double d) { return (char) d; }");
	FUNCTDEF_PARSE_DOUBLE = FUNCT_DEF("double " + FUNCTNAME_PARSE_DOUBLE + "(string s)\n{\n\tif (isdigit(s[0]) || s[0] == '-')\n\t{\n\t\treturn stod(s, NULL);\n\t}\n\treturn 0;\n}");
	FUNCTDEF_ASCII_AT = FUNCT_DEF("double " + FUNCTNAME_ASCII_AT + "(string s, int loc)\n{\n\tif (loc < 0 || loc >= s.length())\n\t{\n\t\treturn 0;\n\t}\n\treturn s.at(loc);\n}");
	FUNCTDEF_LENGTH = FUNCT_DEF("double " + FUNCTNAME_LENGTH + "(string s) { return (emptyString + s).size(); }");
	FUNCTDEF_RETRIEVE_DOUBLE = FUNCT_DEF("double " + FUNCTNAME_RETRIEVE_DOUBLE + "()\n{\n\tdouble d = 0;\n\tcin >> d;\n\treturn d;\n}");
	FUNCTDEF_RETRIEVE_STRING = FUNCT_DEF("string " + FUNCTNAME_RETRIEVE_STRING + "()\n{\n\tstring s = "";\n\tcin >> s;\n\treturn s;\n}");
	FUNCTDEF_RANDOM = FUNCT_DEF("double " + FUNCTNAME_RANDOM + "() { return ((double) rand() / (RAND_MAX)); }");
	FUNCTDEF_READ = FUNCT_DEF("char " + FUNCTNAME_READ + "() { return (char)(in_file.get()); }");
	FUNCTDEF_ELEMENT_AT = FUNCT_DEF("string " + FUNCTNAME_ELEMENT_AT + "(vector<string> v, int index)\n{\n\tif (index < 0 || index >= v.size())\n\t{\n\t\treturn \"\";\n\t}\n\treturn v[index];\n}");
	FUNCTDEF_SIZE = FUNCT_DEF("double " + FUNCTNAME_SIZE + "(vector<string> v) { return v.size(); }");
	FUNCTDEF_DISPLAY_DBL = FUNCT_DEF("void " + CMDNAME_DISPLAY_DBL + "(double d) { cout << d; }");
	FUNCTDEF_DISPLAY_STR = FUNCT_DEF("void " + CMDNAME_DISPLAY_STR + "(string s) { cout << s; }");
	FUNCTDEF_OPEN_I = FUNCT_DEF("void " + CMDNAME_OPEN_INPUT + "(string s) { " + SYM_I_FILE + ".open(s); }");
	FUNCTDEF_OPEN_O = FUNCT_DEF("void " + CMDNAME_OPEN_OUTPUT + "(string s) { " + SYM_O_FILE + ".open(s); }");
	FUNCTDEF_WRITE = FUNCT_DEF("void " + CMDNAME_WRITE + "(string s) { " + SYM_O_FILE + " << s; }");
	FUNCTDEF_CLOSE_I = FUNCT_DEF("void " + CMDNAME_CLOSE_INPUT + "() { " + SYM_I_FILE + ".close(); }");
	FUNCTDEF_CLOSE_O = FUNCT_DEF("void " + CMDNAME_CLOSE_OUTPUT + "() { " + SYM_O_FILE + ".close(); }");

	INIT_ARGS = INIT_STMT(SYM_ARGS + " = vector<string>(argv + 1, argv + argc);");
	INIT_RANDOM = INIT_STMT("srand(time(NULL)); rand();");
	INIT_I_FILE = INIT_STMT(SYM_I_FILE + " = ifstream();");
	INIT_O_FILE = INIT_STMT(SYM_O_FILE + " = ofstream();");

	USING = STATIC_SUPPORT("using namespace std;");
	MAIN_DEC = STATIC_SUPPORT("int main(int argc, char* argv[])");

	initDependencies();
}

// ----------------------------------------------------------
// This function returns the pre-program code
// @structure: the program's structure
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getMetaCode(ProgramStruct * structure)
{
	string result = getUsingStatementText();
	result += getSupportCode();

	result += getBuiltInFunctionCode();
	result += getBuiltInCommandCode();

	result += getForwardDeclarationCode(structure);
	result += getMainFunctionText(structure->PEF->UDFName);
	return result;
}

// ----------------------------------------------------------
// This function returns the PEF code
// @rec: the PEF's record
// @symbols: the PEF's symbol table
// @pefText: the output text of the PEF's statements
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getPEFCode(UDFRecord* rec, SymbolTable* symbols, string pefText)
{
	return getUDFCode(rec, symbols, pefText);
}

// ----------------------------------------------------------
// This function returns the UDF code
// @rec: the UDF's record
// @symbols: the UDF's symbol table
// @udfText: the output text of the UDF's statements
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getUDFCode(UDFRecord* rec, SymbolTable* symbols, string udfText)
{
	string result = "";

	result += line(getFunctionPrototype(rec->returnType, rec->UDFName, rec->args));
	result += openBraceLine();

	result += getSymbolTableCode(symbols);

	result += emptyLine() + emptyLine();

	result += udfText;

	result+= emptyLine();
	result+= closeBraceLine();
	result+= emptyLine();

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for a given jump stmt.
// @udfName: the name of the enclosing UDF
// @stmtNo: the index of this stmt within the enclosing UDF
// @jmpNo: the index of the stmt to jump to following this stmt
// @isOwnLine: a flag for jump stmt nested in if stmts
// @stmtText: the output text from this stmt's contents
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getJmpStmtText(string udfName, int stmtNo, int jmpNo, bool isOwnLine, string stmtText)
{
	string result = "";
		
	if (isOwnLine)
	{
		result += line(getLabelText(udfName, stmtNo) + ":");
		increaseIndent();
				
	}
	result += stmtText;
			
	result += line("goto " + getLabelText(udfName, jmpNo) + ";");
	if (isOwnLine)
	{
		result += emptyLine();
		decreaseIndent();
	}

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for a given if stmt.
// @udfName: the name of the enclosing UDF
// @stmtNo: the index of this stmt within the enclosing UDF
// @isOwnLine: a flag for if stmt nested in if stmts
// @boolExpText: the output text from the boolean exp
// @trueStmtText: the output text from the true stmt
// @falseStmtText: the output text from the false stmt
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getIfStmtText(string udfName, int stmtNo, bool isOwnLine,
								  string boolExpText, string trueStmtText, string falseStmtText)
{
	string result = "";

	if (isOwnLine) 
	{
		result += line(getLabelText(udfName, stmtNo) + ":");
		increaseIndent();
	}

	result += line("if (" + boolExpText + ")");
	result += openBraceLine();

	result += trueStmtText;

	result += closeBraceLine();
	result += line("else");
	result += openBraceLine();
	
	result += falseStmtText;

	result += closeBraceLine();

	if (isOwnLine)
		decreaseIndent();
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for a given assignment.
// @assigneeText: the output text from the assignee
// @assignorText: the output text from the assignor
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getAssignmentText(string assigneeText, string assignorText)
{
	return line(assigneeText + " = " + nest(true, assignorText) + ";");
}

// ----------------------------------------------------------
// This function returns the output text for a function call.
// @isBuiltIn: a flag for a language-defined function
// @primaryText: the output text from the primary object
// @name: the function's name
// @argListText: the output text from the call's arg list
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getFunctionCallText(bool isBuiltIn, string primaryText, string name, string argListText)
{
	string result = "";

	//For User Defined
	//result += (primaryText != "") ? nest(true, primaryText) + "." : "";
	
	if (primaryText == "")
		result += argListText;
	else
	{
		result += primaryText + ((argListText != "") ? ", " + argListText : "");
	}

	return name + nest(true, result);
}

// ----------------------------------------------------------
// This function returns the output text for a command call.
// @isBuiltIn: a flag for language-defined commands
// @primaryText: the output text from the primary object
// @name: the command's name
// @argListText: the output text from the call's arg list
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getCommandCallText(bool isBuiltIn, string primaryText, string name, string argListText)
{
	string result = "";

	if (isBuiltIn && name == CMDNAME_END_NULL)
		return line("return " + argListText + ";");

	//For Built-In
	//result += (primaryText != "") ? nest(true, primaryText) + "." : ""; 

	return name + nest(true, argListText) + ";";
}

// ----------------------------------------------------------
// This function returns the output text for an arg list.
// @thisArgText: the output text for the current argument
// @argTailText: the output text for the remaining tail
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getArgumentListText(string thisArgText, string argTailText)
{
	return thisArgText + ((argTailText != "") ? ", " + argTailText : "");
}

// ----------------------------------------------------------
// This function returns the output text for an operation.
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getBinaryOpText(bool isNested, bool isString, string leftOperandText, string rightOperandText, binaryOp op)
{
	string pretext = "";
	switch (op)
	{
	case ADD:
		pretext = (isString) ? SYM_EMPTY_STRING + " + " : "";
		return getBinaryOperationText(isNested, pretext, " + ", "", leftOperandText, rightOperandText);
	case SUB:
		return getBinaryOperationText(isNested, "", " - ", "", leftOperandText, rightOperandText);
	case MUL:
		return getBinaryOperationText(isNested, "", " * ", "", leftOperandText, rightOperandText);
	case DIV:
		return getBinaryOperationText(isNested, "", " / ", "", leftOperandText, rightOperandText);
	case MODDIV:
		return getBinaryOperationText(isNested, "fmod( ", ", ", ")", leftOperandText, rightOperandText);
	case IDIV:
		return getBinaryOperationText(isNested, "((int)round(",")) / ((int)round(","))", leftOperandText, rightOperandText);
	case ROOT:
		return getBinaryOperationText(isNested, "rt(", ", ", ")", leftOperandText, rightOperandText);
	case EXP:
		return getBinaryOperationText(isNested, "pow(", ", ", ")", leftOperandText, rightOperandText);
	case LT:
		return getBinaryOperationText(isNested, "", " < ", "", leftOperandText, rightOperandText);
	case GT:
		return getBinaryOperationText(isNested, "", " > ", "", leftOperandText, rightOperandText);
	case EQ:
		return getBinaryOperationText(isNested, "", " == ", "", leftOperandText, rightOperandText);
	case LTE:
		return getBinaryOperationText(isNested, "", " <= ", "", leftOperandText, rightOperandText);
	case GTE:
		return getBinaryOperationText(isNested, "", " >= ", "", leftOperandText, rightOperandText);
	}

	return "";
}

string CPPLanguage::getNotOperationText(string notText)
{	return "!" + nest(true, notText); }

// ----------------------------------------------------------
// This function returns the output text for a given identifier.
// @nested: a flag for nesting
// @id: the identifer name
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getIdentifierText(bool nested, string id)
{
	string result = "";

	if (id == SYM_ARGS)
		result += id;
	else
		//prepend identifiers to avoid c++ keyword conflicts
		result += "_" + id;

	return nest(nested, result);
}

//Literals' Output Text
string CPPLanguage::getStringLiteralText(string str)
{ 	return str; }
string CPPLanguage::getDoubleLiteralText(bool isNested, string dbl)
{	return nest(isNested, dbl); }

//SupportReqsPhase Accessors
void CPPLanguage::argsUsed() { SYMDEF_ARGS.needed(); }
void CPPLanguage::toStringUsed() { FUNCTDEF_TO_STRING.needed(); }
void CPPLanguage::toAsciiUsed() { FUNCTDEF_TO_ASCII.needed(); }
void CPPLanguage::parseDoubleUsed() { FUNCTDEF_PARSE_DOUBLE.needed(); }
void CPPLanguage::asciiAtUsed() { FUNCTDEF_ASCII_AT.needed(); }
void CPPLanguage::lengthUsed() { FUNCTDEF_LENGTH.needed(); }
void CPPLanguage::retrieveDoubleUsed() { FUNCTDEF_RETRIEVE_DOUBLE.needed(); }
void CPPLanguage::retrieveStringUsed() { FUNCTDEF_RETRIEVE_STRING.needed(); }
void CPPLanguage::randomUsed() { FUNCTDEF_RANDOM.needed(); }
void CPPLanguage::readUsed() { FUNCTDEF_READ.needed(); }
void CPPLanguage::elementAtUsed() { FUNCTDEF_ELEMENT_AT.needed(); }
void CPPLanguage::sizeUsed() { FUNCTDEF_SIZE.needed(); }
void CPPLanguage::displayDoubleUsed() { FUNCTDEF_DISPLAY_DBL.needed(); }
void CPPLanguage::displayStringUsed() { FUNCTDEF_DISPLAY_STR.needed(); }
void CPPLanguage::openInputUsed() { FUNCTDEF_OPEN_I.needed(); }
void CPPLanguage::closeInputUsed() { FUNCTDEF_CLOSE_I.needed(); }
void CPPLanguage::openOutputUsed() { FUNCTDEF_OPEN_O.needed(); }
void CPPLanguage::writeUsed() { FUNCTDEF_WRITE.needed(); }
void CPPLanguage::closeOutputUsed() { FUNCTDEF_CLOSE_O.needed(); }
void CPPLanguage::addOpUsed() { SYMDEF_EMPTY_STRING.needed(); }
void CPPLanguage::modDivOpUsed() { IMP_MATH.needed(); }
void CPPLanguage::iDivOpUsed() { FUNCTDEF_ROUND.needed(); }
void CPPLanguage::rootOpUsed() { FUNCTDEF_RT.needed(); }
void CPPLanguage::expOpUsed() { IMP_MATH.needed(); }

// ----------------------------------------------------------
// This function returns the output text for a binary operation
// in the following order: 
// pretext, leftOperandText, midtext, rightOperandText, posttext.
// @nested: flag for nesting
//
// Version 4.2
// ---------------------------------------------------------
string CPPLanguage::getBinaryOperationText(bool nested, string pretext, string midtext, string posttext, 
										   string leftOperandText, string rightOperandText)
{	return nest(nested, pretext + leftOperandText + midtext + rightOperandText + posttext); }

// ----------------------------------------------------------
// This function returns the output text for the using statement.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getUsingStatementText()
{
	string result = "";
	result += getSupportText(IMP_IO_STREAM);
	result += getSupportText(IMP_MATH);
	result += getSupportText(IMP_STD_LIB);
	result += getSupportText(IMP_TIME);
	result += getSupportText(IMP_F_STREAM);
	result += getSupportText(IMP_STRING);
	result += getSupportText(IMP_VECTOR);
	result += getSupportText(USING);
	result += emptyLine();
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the global 
// support code (constants and functions).
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getSupportCode()
{
	string result = "";
	result += getSupportText(SYMDEF_ARGS);
	result += getSupportText(SYMDEF_I_FILE);
	result += getSupportText(SYMDEF_O_FILE);
	result += getSupportText(SYMDEF_EMPTY_STRING);
	result += emptyLine();

	result += getSupportText(FUNCTDEF_ROUND);
	result += emptyLine();
	result += getSupportText(FUNCTDEF_RT);
	result += emptyLine();
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the built in functions.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getBuiltInFunctionCode()
{
	string result = "";
	result += getSupportText(FUNCTDEF_TO_STRING);
	result += getSupportText(FUNCTDEF_TO_ASCII);
	result += getSupportText(FUNCTDEF_PARSE_DOUBLE);
	result += getSupportText(FUNCTDEF_ASCII_AT);
	result += getSupportText(FUNCTDEF_LENGTH);
	result += getSupportText(FUNCTDEF_RETRIEVE_DOUBLE);
	result += getSupportText(FUNCTDEF_RETRIEVE_STRING);
	result += getSupportText(FUNCTDEF_RANDOM);
	result += getSupportText(FUNCTDEF_READ);
	result += getSupportText(FUNCTDEF_ELEMENT_AT);
	result += getSupportText(FUNCTDEF_SIZE);
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the built in commands.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getBuiltInCommandCode()
{
	string result = "";
	result += getSupportText(FUNCTDEF_DISPLAY_DBL);
	result += getSupportText(FUNCTDEF_DISPLAY_STR);
	result += getSupportText(FUNCTDEF_OPEN_I);
	result += getSupportText(FUNCTDEF_OPEN_O);
	result += getSupportText(FUNCTDEF_WRITE);
	result += getSupportText(FUNCTDEF_CLOSE_I);
	result += getSupportText(FUNCTDEF_CLOSE_O);
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the forward 
// function declarations.
// @prog: The structure table for the program.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getForwardDeclarationCode(ProgramStruct * prog)
{
	string result = "";
	result += line(getFunctionPrototype(prog->PEF->returnType, prog->PEF->UDFName, prog->PEF->args) + ";");

	for (int index = 0; index < prog->getNumberOfUDFs(); index++)
	{
		UDFRecord * currFunct = prog->getUDF(index);
		result += line(getFunctionPrototype(currFunct->returnType, currFunct->UDFName, currFunct->args) + ";");
	}

	result += emptyLine();
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for main.
// @PEFName: The name of the PEF function.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getMainFunctionText(string PEFName)
{
	string result = "";
	result += getSupportText(MAIN_DEC);
	result += openBraceLine();

	result += getSupportText(INIT_ARGS);
	result += getSupportText(INIT_RANDOM);
	result += getSupportText(INIT_I_FILE);
	result += getSupportText(INIT_O_FILE);

	result += emptyLine();
	result += line(PEFName + nest(true, "") + ";");
	result += closeBraceLine();
	result += emptyLine();
	return result;
}

// ----------------------------------------------------------
// This function adds declarations for each symbol in the 
// given symbol table.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getSymbolTableCode(SymbolTable * symbols)
{
	string result = "";
	for (int i = 0; i < symbols->size(); i++)
	{
		Symbol* s = (*symbols)[i];

		if (!s->isLocal)
			continue;
		
		//emit variable declaration and initialization
		if (s->type == DataType::DT_DOUBLE)
			result += line(DT_DOUBLE + " _" + s->id + " = 0;");
		else if (s->type == DataType::DT_STRING)
			result += line(DT_STRING + " _" + s->id + " = \"\";");
		else
			result += line("Not_Defined _" + s->id + " = NULL;");
	}
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the given support text.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getSupportText(SUPPORT_TEXT& text)
{
	string result = "";
	if (text.isNeeded() && !text.isDefined)
	{
		result += line(text);
		text.isDefined = true;
	}
	return result;
}

// ----------------------------------------------------------
// This function generates the label text for the given index
// in the current UDF.
// @labelIndex: The index of the label.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getLabelText(string udfName, int labelIndex)
{
	return "__LABEL_" + udfName + "_" + to_string(labelIndex);
}

// ----------------------------------------------------------
// This function generates the function prototype of the given
// UDFRecord.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getFunctionPrototype(DataType returnType, string udfName, vector<argPair*>* args)
{
	return getTypeString(returnType) + " " + udfName + nest(true, getArgsString(args));
}

// ----------------------------------------------------------
// This function returns the output text corresponding to
// the given DataType.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getTypeString(DataType dt)
{
	switch (dt)
	{
	case DataType::DT_DOUBLE:
		return DT_DOUBLE;
	case DataType::DT_STRING:
		return DT_STRING;
	case DataType::DT_NULL:
		return DT_VOID;
	default:
		return "UNDEFINED TYPE";
	}
}

// ----------------------------------------------------------
// This function returns the output text for the given
// argument list.
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getArgsString(vector<argPair *> * args)
{
	string result = "";
	int index = 0;

	while (index < args->size())
	{
		argPair * arg = (*args)[index];
		result = result + getTypeString(arg->type) + " _" + arg->name;
		index++;

		if (index != args->size())
			result = result + ", ";
	}

	return result;
}

// ----------------------------------------------------------
// This function initializes the language element dependencies.
//
// Version 4.2
// ----------------------------------------------------------
void CPPLanguage::initDependencies()
{
	SYMDEF_ARGS.addDependency(&IMP_VECTOR);
	SYMDEF_ARGS.addDependency(&IMP_STRING);
	SYMDEF_ARGS.addDependency(&INIT_ARGS);

	SYMDEF_I_FILE.addDependency(&IMP_F_STREAM);
	SYMDEF_I_FILE.addDependency(&INIT_I_FILE);

	SYMDEF_O_FILE.addDependency(&IMP_F_STREAM);
	SYMDEF_O_FILE.addDependency(&INIT_O_FILE);

	SYMDEF_EMPTY_STRING.addDependency(&IMP_STRING);

	FUNCTDEF_ROUND.addDependency(&IMP_MATH);

	FUNCTDEF_RT.addDependency(&IMP_MATH);

	//TO_ASCII has no dependencies

	FUNCTDEF_TO_STRING.addDependency(&IMP_STRING);
	FUNCTDEF_PARSE_DOUBLE.addDependency(&IMP_STRING);
	FUNCTDEF_ASCII_AT.addDependency(&IMP_STRING);
	FUNCTDEF_LENGTH.addDependency(&IMP_STRING);

	FUNCTDEF_RETRIEVE_DOUBLE.addDependency(&IMP_IO_STREAM);
	FUNCTDEF_RETRIEVE_STRING.addDependency(&IMP_IO_STREAM);
	FUNCTDEF_RETRIEVE_STRING.addDependency(&IMP_STRING);
	
	FUNCTDEF_RANDOM.addDependency(&IMP_STD_LIB);
	FUNCTDEF_RANDOM.addDependency(&IMP_TIME);
	FUNCTDEF_RANDOM.addDependency(&INIT_RANDOM);
	
	FUNCTDEF_READ.addDependency(&SYMDEF_I_FILE);
	
	FUNCTDEF_ELEMENT_AT.addDependency(&IMP_VECTOR);
	FUNCTDEF_ELEMENT_AT.addDependency(&IMP_STRING);
	FUNCTDEF_SIZE.addDependency(&IMP_VECTOR);
	FUNCTDEF_SIZE.addDependency(&IMP_STRING);
	
	FUNCTDEF_DISPLAY_DBL.addDependency(&IMP_IO_STREAM);
	FUNCTDEF_DISPLAY_STR.addDependency(&IMP_IO_STREAM);
	FUNCTDEF_DISPLAY_STR.addDependency(&IMP_STRING);
	
	FUNCTDEF_OPEN_I.addDependency(&IMP_STRING);
	FUNCTDEF_OPEN_I.addDependency(&SYMDEF_I_FILE);
	FUNCTDEF_OPEN_O.addDependency(&IMP_STRING);
	FUNCTDEF_OPEN_O.addDependency(&SYMDEF_O_FILE);
	
	FUNCTDEF_WRITE.addDependency(&IMP_STRING);
	FUNCTDEF_WRITE.addDependency(&SYMDEF_O_FILE);
	
	FUNCTDEF_CLOSE_I.addDependency(&SYMDEF_I_FILE);
	FUNCTDEF_CLOSE_O.addDependency(&SYMDEF_O_FILE);
}