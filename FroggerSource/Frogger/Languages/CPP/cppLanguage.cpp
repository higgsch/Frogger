//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.3
// -----------------------------------------------------------------
// This program represents the CPPLanguage package.
// -----------------------------------------------------------------
#include "cppLanguage.h"
using namespace std;

extern string rootDir;

// ----------------------------------------------------------
// This function calls a Windows module to compile the temp
// C++ code to .exe.
// @outFilename: The name of the .cpp file
// @exeFilename: The name of the desired .exe output file.
//
// Version 4.3
// ----------------------------------------------------------
void CPPLanguage::outputToExe(string outFilename, string exeFilename)
{
	int status = system(("call " + rootDir + "Languages\\CPP\\MSCL.bat " + outFilename + " " + exeFilename + ">nul").c_str());
	
	if (status != 0)
		cpp_error("12-0014");
}

// ----------------------------------------------------------
// This function calls a Windows module to clean up all temp
// files created from compilation.
// @filename: The name of the temporary .cpp file
//
// Version 4.3
// ----------------------------------------------------------
void CPPLanguage::cleanup(string filename)
{
	system(("call " + rootDir + "Languages\\CPP\\clean.bat " + filename + ">nul").c_str());
}

const string CPPLanguage::OPEN_BRACE = "{";
const string CPPLanguage::CLOSE_BRACE = "}";
const string CPPLanguage::OPEN_NEST = "(";
const string CPPLanguage::CLOSE_NEST = ")";
const string CPPLanguage::INDENT = "\t";

// ----------------------------------------------------------
// This function initializes the language elements.
//
// Version 5.3
// ----------------------------------------------------------
void CPPLanguage::initOutputTexts()
{
	DT_DOUBLE = OUTPUT_TEXT("_double");
	DT_STRING = OUTPUT_TEXT("_string");
	DT_LIST = OUTPUT_TEXT("_list");
	DT_STRINGLIST = OUTPUT_TEXT("_stringList");
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
	FUNCTNAME_ADD = OUTPUT_TEXT("add");
	FUNCTNAME_REMOVE = OUTPUT_TEXT("remove");

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

	SYMDEF_ARGS = VAR_DEF(DT_STRINGLIST + " " + SYM_ARGS + ";");
	SYMDEF_I_FILE = VAR_DEF("ifstream " + SYM_I_FILE + ";");
	SYMDEF_O_FILE = VAR_DEF("ofstream " + SYM_O_FILE + ";");
	SYMDEF_EMPTY_STRING = VAR_DEF("const " + DT_STRING + " " + SYM_EMPTY_STRING + " = \"\";");

	string doubleDef = "struct " + DT_DOUBLE + "\n{\n";
	doubleDef += "\tdouble val;\n";
	doubleDef += "\t" + DT_DOUBLE + "() { val = 0; }\n";
	doubleDef += "\t" + DT_DOUBLE + "(double d) { val = d; }\n\n";
	doubleDef += getLocalOperatorOverloadText(DT_DOUBLE);
	doubleDef += "\tfriend " + DT_DOUBLE + " pow(" + DT_DOUBLE + " lhs, const " + DT_DOUBLE + "& rhs){ ";
	doubleDef += "lhs.val = pow(lhs.val, rhs.val); return lhs; }\n";
	doubleDef += "\t" + DT_STRING + " _" +  FUNCTNAME_TO_STRING + "();\n";
	doubleDef += "\tchar _" +  FUNCTNAME_TO_ASCII + "();\n";
	doubleDef += "};\n";
	OBJDEF_DOUBLE = OBJ_DEF(doubleDef);

	string stringDef = "struct " + DT_STRING + "\n{\n";
	stringDef += "\tstring val;\n";
	stringDef += "\t" + DT_STRING + "() { val = \"\"; }\n";
	stringDef += "\t" + DT_STRING + "(string s) { val = s; }\n\n";
	stringDef += getLocalOperatorOverloadText(DT_STRING);
	stringDef += "\t" + DT_DOUBLE + " at(int loc) { return val.at(loc); }\n";
	stringDef += "\t" + DT_DOUBLE + " _" + FUNCTNAME_PARSE_DOUBLE + "();\n";
	stringDef += "\t" + DT_DOUBLE + " _" + FUNCTNAME_ASCII_AT + "(" + DT_DOUBLE + " loc);\n";
	stringDef += "\t" + DT_DOUBLE + " _" + FUNCTNAME_LENGTH + "();\n";
	stringDef += "};\n";
	OBJDEF_STRING = OBJ_DEF(stringDef);

	string listDef = "template <class _O>\nstruct " + DT_LIST + "\n{\n";
	listDef += "\tvector<_O> myList;\n";
	listDef += "\t_O _" + FUNCTNAME_ELEMENT_AT + "(" + DT_DOUBLE + " index);\n";
	listDef += "\t" + DT_DOUBLE + " _" + FUNCTNAME_SIZE + "();\n";
	listDef += "\tvoid _" + FUNCTNAME_ADD + "(_O toAdd);\n";
	listDef += "\tvoid _" + FUNCTNAME_REMOVE + "(" + DT_DOUBLE + " loc);\n";
	listDef += "};\n";
	OBJDEF_LIST = OBJ_DEF(listDef);

	OBJDEF_STRINGLIST = OBJ_DEF("struct " + DT_STRINGLIST + " : public " + DT_LIST + "<" + DT_STRING + ">{};\n");

	FUNCTDEF_TO_STRING = FUNCT_DEF(DT_STRING + " " + DT_DOUBLE + "::_" + FUNCTNAME_TO_STRING + "() { return to_string(val); }");
	FUNCTDEF_TO_ASCII = FUNCT_DEF("char " + DT_DOUBLE + "::_" + FUNCTNAME_TO_ASCII + "() { return (char) val; }");
	FUNCTDEF_PARSE_DOUBLE = FUNCT_DEF(DT_DOUBLE + " " + DT_STRING + "::_" + FUNCTNAME_PARSE_DOUBLE + "()\n{\n\tif (isdigit(val[0]) || val[0] == '-')\n\t{\n\t\treturn stod(val, NULL);\n\t}\n\treturn 0;\n}");
	FUNCTDEF_ASCII_AT = FUNCT_DEF(DT_DOUBLE + " " + DT_STRING + "::_" + FUNCTNAME_ASCII_AT + "(" + DT_DOUBLE + " loc)\n{\n\tif (loc < 0 || loc >= val.length())\n\t\treturn 0;\n\treturn val.at(loc.val);\n}");
	FUNCTDEF_LENGTH = FUNCT_DEF(DT_DOUBLE + " " + DT_STRING + "::_" + FUNCTNAME_LENGTH + "() { return val.length(); }");
	FUNCTDEF_ELEMENT_AT = FUNCT_DEF("template<class _O>\n_O " + DT_LIST + "<_O>::_" + FUNCTNAME_ELEMENT_AT + "(" + DT_DOUBLE + " index)\n{\n\tif (index < 0 || index >= myList.size())\n\t\treturn \"\";\n\treturn myList[index.val];\n}");
	FUNCTDEF_SIZE = FUNCT_DEF("template<class _O>\n" + DT_DOUBLE + " " + DT_LIST + "<_O>::_" + FUNCTNAME_SIZE + "() { return myList.size(); }");
	FUNCTDEF_ADD = FUNCT_DEF("template<class _O>\nvoid " + DT_LIST + "<_O>::_" + FUNCTNAME_ADD + "(_O toAdd) { myList.push_back(toAdd); }");
	FUNCTDEF_REMOVE = FUNCT_DEF("template<class _O>\nvoid " + DT_LIST + "<_O>::_" + FUNCTNAME_REMOVE + "(" + DT_DOUBLE + " loc) { myList.erase(loc.val); }");

	FUNCTDEF_ROUND = FUNCT_DEF(DT_DOUBLE + " round(" + DT_DOUBLE + " num) {\n\treturn (num > 0.0) ? floor(num.val + 0.5) : ceil(num.val - 0.5);\n}");
	FUNCTDEF_RT = FUNCT_DEF(DT_DOUBLE + " rt(" + DT_DOUBLE + " l, " + DT_DOUBLE + " r) {\n\t return pow(r.val, 1.0 / l.val);\n}");
	FUNCTDEF_RETRIEVE_DOUBLE = FUNCT_DEF(DT_DOUBLE + " " + FUNCTNAME_RETRIEVE_DOUBLE + "()\n{\n\tdouble d = 0;\n\tcin >> d;\n\treturn d;\n}");
	FUNCTDEF_RETRIEVE_STRING = FUNCT_DEF(DT_STRING + " " + FUNCTNAME_RETRIEVE_STRING + "()\n{\n\tstring s = "";\n\tcin >> s;\n\treturn s;\n}");
	FUNCTDEF_RANDOM = FUNCT_DEF(DT_DOUBLE + " " + FUNCTNAME_RANDOM + "() { return ((double) rand() / (RAND_MAX)); }");
	FUNCTDEF_READ = FUNCT_DEF(DT_STRING + " " + FUNCTNAME_READ + "() { string result; result = (char)(in_file.get()); return result; }");
	FUNCTDEF_DISPLAY_DBL = FUNCT_DEF("void " + CMDNAME_DISPLAY_DBL + "(" + DT_DOUBLE + " d) { cout << d.val; }");
	FUNCTDEF_DISPLAY_STR = FUNCT_DEF("void " + CMDNAME_DISPLAY_STR + "(" + DT_STRING + " s) { cout << s.val; }");
	FUNCTDEF_OPEN_I = FUNCT_DEF("void " + CMDNAME_OPEN_INPUT + "(" + DT_STRING + " s) { " + SYM_I_FILE + ".open(s.val); }");
	FUNCTDEF_OPEN_O = FUNCT_DEF("void " + CMDNAME_OPEN_OUTPUT + "(" + DT_STRING + " s) { " + SYM_O_FILE + ".open(s.val); }");
	FUNCTDEF_WRITE = FUNCT_DEF("void " + CMDNAME_WRITE + "(" + DT_STRING + " s) { " + SYM_O_FILE + " << s.val; }");
	FUNCTDEF_CLOSE_I = FUNCT_DEF("void " + CMDNAME_CLOSE_INPUT + "() { " + SYM_I_FILE + ".close(); }");
	FUNCTDEF_CLOSE_O = FUNCT_DEF("void " + CMDNAME_CLOSE_OUTPUT + "() { " + SYM_O_FILE + ".close(); }");

	string argsInit = "vector<string> argList(argv + 1, argv + argc);\n";
	argsInit += "\tint argCount = argList.size();\n";
	argsInit += "\tfor (int argIndex = 0; argIndex < argCount; argIndex++){\n";
	argsInit += "\t\t" + SYM_ARGS + "._" + FUNCTNAME_ADD + "(argList[argIndex]);\n\t}\n";
	INIT_ARGS = INIT_STMT(argsInit);
	INIT_RANDOM = INIT_STMT("srand(time(NULL)); rand();");
	INIT_I_FILE = INIT_STMT(SYM_I_FILE + " = ifstream();");
	INIT_O_FILE = INIT_STMT(SYM_O_FILE + " = ofstream();");

	USING = STATIC_SUPPORT("using namespace std;");
	MAIN_DEC = STATIC_SUPPORT("int main(int argc, char* argv[])");

	initDependencies();
	
	dt_doubleUsed();
	dt_stringUsed();
	dt_listUsed();
	dt_stringListUsed();
}

// ----------------------------------------------------------
// This function returns the pre-program code
// @structure: the program's structure
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getMetaCode(ProgramStruct * structure)
{
	string result = getUsingStatementText();

	result += getBuiltInObjectCode();
	result += getSupportCode();
	result += getBuiltInFunctionCode();
	result += getBuiltInCommandCode();

	result += getForwardDeclarationCode(structure);
	result += getClassDefinitionCode(structure);

	result += emptyLine();
	for (int dataIndex = 0; dataIndex < structure->data->size(); dataIndex++)
	{
		DataRecord * currData = structure->data->at(dataIndex);
		result += line(getTypeString(currData->type) + " _" + currData->memberName + ";");
	}

	result += getMainFunctionText(structure->PEF->name);
	return this->clearIndents(result);
}

// ----------------------------------------------------------
// This function returns the PEF code
// @rec: the PEF's record
// @pefText: the output text of the PEF's statements
//
// Version 4.2
// ----------------------------------------------------------
string CPPLanguage::getPEFCode(UDFRecord* rec, string pefText)
{
	return getUDFCode(rec, pefText);
}

// ----------------------------------------------------------
// This function returns the UDF code
// @rec: the UDF's record
// @udfText: the output text of the UDF's statements
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getUDFCode(UDFRecord* rec, string udfText)
{
	string result = emptyLine();
	
	result += line(getFunctionPrototype(rec));
	result += openBraceLine();

	string udfContents = getSymbolTableCode(rec->visibleTables->syms);
	udfContents += emptyLine();
	udfContents += udfText;
	result += increaseIndent(udfContents);

	result+= closeBraceLine();

	return clearIndents(result);
}

// ----------------------------------------------------------
// This function returns the output text for a given jump stmt.
// @udfName: the name of the enclosing UDF
// @stmtNo: the index of this stmt within the enclosing UDF
// @jmpNo: the index of the stmt to jump to following this stmt
// @isOwnLine: a flag for jump stmt nested in if stmts
// @stmtText: the output text from this stmt's contents
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getJmpStmtText(string udfName, int stmtNo, int jmpNo, bool isOwnLine, string stmtText)
{
	string result = "";
		
	if (isOwnLine)
	{
		result += emptyLine() + line(getLabelText(udfName, stmtNo) + ":");
	}

	string stmtContents = line(stmtText);
	stmtContents += line("goto " + getLabelText(udfName, jmpNo) + ";");

	if (isOwnLine)
	{
		stmtContents = increaseIndent(stmtContents);
	}

	result += stmtContents;

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
// Version 5.0
// ---------------------------------------------------------
string CPPLanguage::getIfStmtText(string udfName, int stmtNo, bool isOwnLine,
								  string boolExpText, string trueStmtText, string falseStmtText)
{
	string result = "";

	if (isOwnLine) 
	{
		result += line(getLabelText(udfName, stmtNo) + ":");
	}

	string ifContents = line("if (" + boolExpText + ")");
	ifContents += openBraceLine();

	ifContents += increaseIndent(trueStmtText);

	ifContents += closeBraceLine();
	ifContents += line("else");
	ifContents += openBraceLine();
	
	ifContents += increaseIndent(falseStmtText);

	ifContents += closeBraceLine();

	if (isOwnLine)
		ifContents = increaseIndent(ifContents);

	result += ifContents;

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for a given assignment.
// @assigneeText: the output text from the assignee
// @assignorText: the output text from the assignor
//
// Version 5.0
// ---------------------------------------------------------
string CPPLanguage::getAssignmentText(string assigneeText, string assignorText)
{
	return assigneeText + " = " + nest(true, assignorText) + ";";
}

// ----------------------------------------------------------
// This function returns the output text for a function call.
// @isBuiltIn: a flag for a language-defined function
// @isParentScoped: a flag for parent scope operator usage.
// @parentType: the data type of primary's parent
// @primaryText: the output text from the primary object
// @name: the function's name
// @argListText: the output text from the call's arg list
//
// Version 5.3
// ---------------------------------------------------------
string CPPLanguage::getFunctionCallText(bool isBuiltIn, bool isParentScoped, DataType * parentType, 
										string primaryText, string name, string argListText)
{
	string result = "";

	if (isBuiltIn && name != FUNCTNAME_TO_STRING && name != FUNCTNAME_TO_ASCII && name != FUNCTNAME_ASCII_AT
		&& name != FUNCTNAME_LENGTH && name != FUNCTNAME_PARSE_DOUBLE
		&& name != FUNCTNAME_ELEMENT_AT	&& name != FUNCTNAME_SIZE
		&& name != FUNCTNAME_ADD && name != FUNCTNAME_REMOVE)
	{
		if (primaryText == "")
			result += argListText;
		else
			result += primaryText + ((argListText != "") ? ", " + argListText : "");

		return name + nest(true, result);
	}

	//For User Defined
	string primary = "";
	if (!isParentScoped)
	{
		primary = (primaryText != "") ? primaryText + "." : "";
	}
	else
	{
		primary = "((" + getTypeString(parentType) + ") ";
		primary += (primaryText == "") ? "*this" : primaryText;
		primary += ").";
	}

	return primary + "_" + name + nest(true, argListText);
}

// ----------------------------------------------------------
// This function returns the output text for a command call.
// @isBuiltIn: a flag for language-defined commands
// @isParentScoped: a flag for parent scope resolution usage
// @parentType: the data type of primary's parent
// @primaryText: the output text from the primary object
// @name: the command's name
// @argListText: the output text from the call's arg list
//
// Version 5.1
// ---------------------------------------------------------
string CPPLanguage::getCommandCallText(bool isBuiltIn, bool isParentScoped, DataType* parentType,
									   string primaryText, string name, string argListText)
{
	if (isBuiltIn && name == CMDNAME_END_NULL)
		return "return" + ((argListText != "") ? " " + argListText : "") + ";";

	if (isBuiltIn)
		return name + nest(true, argListText) + ";";

	//For User Defined
	string primary = "";
	if (!isParentScoped)
	{
		primary = (primaryText != "") ? primaryText + "." : "";
	}
	else
	{
		primary = "((" + getTypeString(parentType) + ") ";
		primary += (primaryText == "") ? "*this" : primaryText;
		primary += ").";
	}

	return primary + "_" + name + nest(true, argListText) + ";";
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
{ 	return DT_STRING + "(" + str + ")"; }
string CPPLanguage::getDoubleLiteralText(bool isNested, string dbl)
{	return nest(isNested, dbl); }

//SupportReqsPhase Accessors
void CPPLanguage::dt_doubleUsed() { OBJDEF_DOUBLE.needed(); }
void CPPLanguage::dt_stringUsed() { OBJDEF_STRING.needed(); }
void CPPLanguage::dt_listUsed() { OBJDEF_LIST.needed(); }
void CPPLanguage::dt_stringListUsed() { OBJDEF_STRINGLIST.needed(); }
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
// This function returns the output text for the built in objects.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getBuiltInObjectCode()
{
	string result = "";
	result += "struct " + DT_STRING + ";\n";
	result += getSupportText(OBJDEF_DOUBLE);
	result += getSupportText(OBJDEF_STRING);
	result += getSupportText(OBJDEF_LIST);
	result += getSupportText(OBJDEF_STRINGLIST);
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for the built in functions.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getBuiltInFunctionCode()
{
	string result = "";
	result += getSupportText(FUNCTDEF_TO_STRING);
	result += getSupportText(FUNCTDEF_TO_ASCII);
	result += getSupportText(FUNCTDEF_PARSE_DOUBLE);
	result += getSupportText(FUNCTDEF_ASCII_AT);
	result += getSupportText(FUNCTDEF_LENGTH);
	result += getSupportText(FUNCTDEF_ELEMENT_AT);
	result += getSupportText(FUNCTDEF_SIZE);
	result += getSupportText(FUNCTDEF_ADD);
	result += getSupportText(FUNCTDEF_REMOVE);

	result += getSupportText(FUNCTDEF_RETRIEVE_DOUBLE);
	result += getSupportText(FUNCTDEF_RETRIEVE_STRING);
	result += getSupportText(FUNCTDEF_RANDOM);
	result += getSupportText(FUNCTDEF_READ);
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
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getForwardDeclarationCode(ProgramStruct * prog)
{
	string result = emptyLine();

	for (int objectIndex = 0; objectIndex < prog->getNumberOfOFs(); objectIndex++)
	{
		ObjectStruct * currObject = prog->getOF(objectIndex);
		if (!currObject->isUserDefined)
			continue;

		result += line(getClassForwardDeclaration(currObject));
	}
	result += emptyLine();

	result += line(getFunctionPrototype(prog->PEF) + ";");

	for (int UDFIndex = 0; UDFIndex < prog->getNumberOfUDFs(); UDFIndex++)
	{
		UDFRecord * currFunct = prog->getUDF(UDFIndex);
		result += line(getFunctionPrototype(currFunct) + ";");
	}

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for all the class 
// definitions within the program.
// @prog: The structure table for the program.
//
// Version 5.1
// ----------------------------------------------------------
string CPPLanguage::getClassDefinitionCode(ProgramStruct * prog)
{
	string result = "";
	bool hasSkipped = false;

	int objCount = prog->getNumberOfOFs();
	do
	{
		hasSkipped = false;
		for (int objIndex = 0; objIndex < objCount; objIndex++)
		{
			ObjectStruct * currObj = prog->getOF(objIndex);
			if (!currObj->isUserDefined)
				continue;

			if (!currObj->isCodeGenerated)
			{
				if (isParentCodeGenerated(currObj))
				{
					result += getClassDefinitionCode(prog->getOF(objIndex));
					currObj->isCodeGenerated = true;
				}
				else
					hasSkipped = true;
			}
		}
	} while (hasSkipped);

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for all contained 
// class definitions.
// @obj: The structure table for the class.
//
// Version 5.2
// ----------------------------------------------------------
string CPPLanguage::getClassDefinitionCode(ObjectStruct * obj)
{
	string result = emptyLine();

	result += templateLine(obj);

	string classDeclaration = "class _" + obj->name;
	if (obj->hasParent())
		classDeclaration += " : public _" + replaceAll(obj->parentType->name,":",":_");

	result += line(classDeclaration);

	result += openBraceLine();

	result += line("public:");
	string ofs = "";
	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		ObjectStruct * currObj = obj->getOF(objIndex);
		if (!currObj->isUserDefined)
			continue;

		ofs += getClassDefinitionCode(obj->getOF(objIndex));
	}
	result += increaseIndent(ofs);
	result += emptyLine();

	string data = "";
	int dataCount = obj->data->size();
	if (dataCount > 0)
		result += line("private:");
	for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
	{
		DataRecord * currData = obj->data->at(dataIndex);
		data += line(getTypeString(currData->type) + " _" + currData->memberName + ";");
	}
	result += increaseIndent(data) + emptyLine();

	if (dataCount > 0)
		result += line("public:");
	string udfs = line(getConstructor(obj));
	int udfCount = obj->getNumberOfUDFs();
	for (int udfIndex = 0; udfIndex < udfCount; udfIndex++)
	{
		UDFRecord * currUDF = obj->getUDF(udfIndex);
		udfs += line(getFunctionDeclaration(currUDF) + ";");
	}
	result += increaseIndent(udfs);
	
	result += line(CLOSE_BRACE + ";");

	return result;
}

// ----------------------------------------------------------
// This function returns the output text for main.
// @PEFName: The name of the PEF function.
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getMainFunctionText(string PEFName)
{
	string result = emptyLine();
	result += getSupportText(MAIN_DEC);
	result += openBraceLine();

	string mainText = getSupportText(INIT_ARGS);
	mainText += getSupportText(INIT_RANDOM);
	mainText += getSupportText(INIT_I_FILE);
	mainText += getSupportText(INIT_O_FILE);

	mainText += line("_" + PEFName + nest(true, "") + ";");

	result += increaseIndent(mainText);
	result += closeBraceLine();
	return result;
}

// ----------------------------------------------------------
// This function returns the output text for an object.
// @obj: The object to output.
//
// Version 5.2
// ----------------------------------------------------------
string CPPLanguage::getClassForwardDeclaration(ObjectStruct * obj)
{
	string result = templateLine(obj);
	result += "class _" + obj->name + ";";
	return result;
}

// ----------------------------------------------------------
// This function adds declarations for each symbol in the 
// given symbol table.
//
// Version 5.0
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
		else if (s->type->type == DTE_USER_DEFINED)
			result += line("_" + replaceAll(s->type->fullyScopedTypeString(), ":", "::_") + " _" + s->id + ";");
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
// This function generates the constructor for the given class.
// @obj: The class struct.
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getConstructor(ObjectStruct * obj)
{
	string result = "_" + obj->name + "() ";
	
	int dataCount = obj->data->size();
	if (dataCount > 0)
		result += ": ";
	for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
	{
		DataRecord * currData = obj->data->at(dataIndex);
		if (currData->type->isUserDefined())
			continue;
		result += "_" + currData->memberName + "(" + currData->defaultValue + "), ";
	}

	//Remove fencepost comma
	if (dataCount > 0)
		result = result.substr(0, result.length() - 2) + " ";

	result += "{}";
	return result;
}

// ----------------------------------------------------------
// This function generates the function prototype of the given
// UDFRecord without scope resolution.
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getFunctionDeclaration(UDFRecord * udf)
{
	return getTypeString(udf->returnType) + " _" + udf->name + 
		nest(true, getArgsString(udf->args));
}

// ----------------------------------------------------------
// This function generates the function prototype of the given
// UDFRecord with scope resolution.
//
// Version 5.2
// ----------------------------------------------------------
string CPPLanguage::getFunctionPrototype(UDFRecord * udf)
{
	string result = "";
	result += templateLine(udf->containingOF);

	result += getTypeString(udf->returnType) + " " + 
		((udf->primary->isNull()) ? "" : getTypeString(udf->primary) + "::") + "_" + udf->name + 
		nest(true, getArgsString(udf->args));

	return result;
}

// ----------------------------------------------------------
// This function returns the output text corresponding to
// the given DataType.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getTypeString(DataType * dt)
{
	switch (dt->type)
	{
	case DTE_DOUBLE:
		return DT_DOUBLE;
	case DTE_STRING:
		return DT_STRING;
	case DTE_LIST:
		return DT_LIST + "<" + getTypeString(dt->templatizerList->at(0)) + ">";
	case DTE_STRINGLIST:
		return DT_STRINGLIST;
	case DTE_NULL:
		return DT_VOID;
	case DTE_USER_DEFINED:
		{
			string typeString = dt->name;
			if (dt->isTemplatized())
			{
				typeString += "<" + getTypeString(dt->templatizerList->at(0));

				int tCount = dt->templatizerList->size();
				for (int tIndex = 1; tIndex < tCount; tIndex++)
					typeString += ", " + getTypeString(dt->templatizerList->at(tIndex));

				typeString += ">";
			}
			return "_" + replaceAll(dt->scope,":","::_") + typeString;
		}
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
string CPPLanguage::getArgsString(ArgList * args)
{
	string result = "";

	int numArgs = args->size();
	for (int index = 0; index < numArgs; index++)
	{
		ArgPair * arg = (*args)[index];
		result = result + getTypeString(arg->type) + " _" + arg->name;

		if (index != numArgs - 1)
			result = result + ", ";
	}

	return result;
}

// ----------------------------------------------------------
// This function returns the templatization line for the given
// object.
//
// Version 5.2
// ----------------------------------------------------------
string CPPLanguage::templateLine(ObjectStruct * obj)
{
	if (obj == NULL || !obj->isTemplated())
		return "";

	string templateLine = "template <class _" + obj->templateList->at(0);

	int tCount = obj->templateList->size();
	for (int tIndex = 1; tIndex < tCount; tIndex++)
	{
		templateLine += ", _" + obj->templateList->at(tIndex);
	}

	templateLine += ">";

	return line(templateLine);
}

// ----------------------------------------------------------
// This function generates the code for operator overloading
// for the given object.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getLocalOperatorOverloadText(OUTPUT_TEXT type)
{
	string text = getArithmeticOperatorOverloadText(type, "+");
	text += getComparisonOperatorOverloadText(type, "==");
	text += getComparisonOperatorOverloadText(type, "!=");

	if (type == DT_STRING)
		return text;

	text += getArithmeticOperatorOverloadText(type, "-");
	text += getArithmeticOperatorOverloadText(type, "/");
	text += getArithmeticOperatorOverloadText(type, "*");
	text += getComparisonOperatorOverloadText(type, "<");
	text += getComparisonOperatorOverloadText(type, ">");
	text += getComparisonOperatorOverloadText(type, "<=");
	text += getComparisonOperatorOverloadText(type, ">=");
	return text;
}

// ----------------------------------------------------------
// This function generates the code for arithmetic operator 
// overloading for the given object and operator.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getArithmeticOperatorOverloadText(OUTPUT_TEXT type, string op)
{
	string text = "\t" + type + "& operator" + op + "=(const " + type + "& rhs){ ";
	text += "val " + op + "= rhs.val; return *this; }\n";
	text += "\tfriend " + type + " operator" + op + "(" + type + " lhs, const " + type + "& rhs){ ";
	text += "lhs " + op + "= rhs; return lhs;}\n";
	return text;
}

// ----------------------------------------------------------
// This function generates the code for comparison operator 
// overloading for the given object and operator.
//
// Version 5.3
// ----------------------------------------------------------
string CPPLanguage::getComparisonOperatorOverloadText(OUTPUT_TEXT type, string op)
{
	string text = "\tfriend bool operator" + op + "(const " + type + "& lhs, const " + type + "& rhs){ ";
	text += "return lhs.val " + op + " rhs.val; }\n";
	return text;
}

// ----------------------------------------------------------
// This function initializes the language element dependencies.
//
// Version 5.3
// ----------------------------------------------------------
void CPPLanguage::initDependencies()
{
	//DOUBLE has no dependencies

	OBJDEF_STRING.addDependency(&IMP_STRING);

	OBJDEF_LIST.addDependency(&IMP_VECTOR);
	OBJDEF_LIST.addDependency(&FUNCTDEF_ADD);
	OBJDEF_LIST.addDependency(&FUNCTDEF_REMOVE);

	OBJDEF_STRINGLIST.addDependency(&OBJDEF_LIST);
	OBJDEF_STRINGLIST.addDependency(&OBJDEF_STRING);

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