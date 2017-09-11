//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.1
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
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getMetaCode(ProgramStruct * structure)
{
	string result = getUsingStatementText();
	result += getSupportCode();

	result += getBuiltInFunctionCode();
	result += getBuiltInCommandCode();

	result += getForwardDeclarationCode(structure);
	result += getClassDefinitionCode(structure);
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
// Version 5.1
// ---------------------------------------------------------
string CPPLanguage::getFunctionCallText(bool isBuiltIn, bool isParentScoped, DataType * parentType, 
										string primaryText, string name, string argListText)
{
	string result = "";

	if (isBuiltIn)
	{
		if (primaryText == "")
			result += argListText;
		else
		{
			result += primaryText + ((argListText != "") ? ", " + argListText : "");
		}

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
// Version 5.1
// ----------------------------------------------------------
string CPPLanguage::getClassDefinitionCode(ObjectStruct * obj)
{
	string result = emptyLine();

	string classDeclaration = "class _" + obj->name;
	if (obj->hasParent())
		classDeclaration += " : public _" + replaceAll(obj->parentName,":",":_");

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
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getClassForwardDeclaration(ObjectStruct * obj)
{
	string result = "class _" + obj->name + ";";
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
			result += line("_" + replaceAll(s->type->typeString, ":", "::_") + " _" + s->id + ";");
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
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getFunctionPrototype(UDFRecord * udf)
{
	return getTypeString(udf->returnType) + " " + 
		((udf->primary->isNull()) ? "" : getTypeString(udf->primary) + "::") + "_" + udf->name + 
		nest(true, getArgsString(udf->args));
}

// ----------------------------------------------------------
// This function returns the output text corresponding to
// the given DataType.
//
// Version 5.0
// ----------------------------------------------------------
string CPPLanguage::getTypeString(DataType * dt)
{
	switch (dt->type)
	{
	case DTE_DOUBLE:
		return DT_DOUBLE;
	case DTE_STRING:
		return DT_STRING;
	case DTE_NULL:
		return DT_VOID;
	case DTE_USER_DEFINED:
		return "_" + replaceAll(dt->typeString,":","::_");
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