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
	//result += buffer.getNext();
			
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

	if (isBuiltIn && name == CommandTable::CMD_END_NULL->name)
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

//Arithmetic Operation Output Texts
string CPPLanguage::getAddOperationText(bool isNested, bool isString, string leftOperandText, string rightOperandText)
{
	string pretext = (isString) ? SYMBOL_TEXT::EMPTY_STRING.getText() + " + " : "";
	return getBinaryOperationText(isNested, pretext, " + ", "", leftOperandText, rightOperandText);
}
string CPPLanguage::getSubOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " - ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getMulOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " * ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getDivOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " / ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getModDivOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "fmod( ", ", ", ")", leftOperandText, rightOperandText); }
string CPPLanguage::getIDivOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "((int)round(",")) / ((int)round(","))", leftOperandText, rightOperandText); }
string CPPLanguage::getRootOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "rt(", ", ", ")", leftOperandText, rightOperandText); }
string CPPLanguage::getExpOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "pow(", ", ", ")", leftOperandText, rightOperandText); }

//Boolean Operation Output Texts
string CPPLanguage::getLTOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " < ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getGTOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " > ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getEQOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " == ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getLTEOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " <= ", "", leftOperandText, rightOperandText); }
string CPPLanguage::getGTEOperationText(bool isNested, string leftOperandText, string rightOperandText)
{	return getBinaryOperationText(isNested, "", " >= ", "", leftOperandText, rightOperandText); }
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

	if (id == SYMBOL_TEXT::ARGS.getText())
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
	result += getSupportText(IMPORT_STMT::IO_STREAM);
	result += getSupportText(IMPORT_STMT::MATH);
	result += getSupportText(IMPORT_STMT::STD_LIB);
	result += getSupportText(IMPORT_STMT::TIME);
	result += getSupportText(IMPORT_STMT::F_STREAM);
	result += getSupportText(IMPORT_STMT::STRING);
	result += getSupportText(IMPORT_STMT::VECTOR);
	result += getSupportText(STATIC_SUPPORT::USING);
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
	result += getSupportText(VAR_DEF::ARGS);
	result += getSupportText(VAR_DEF::I_FILE);
	result += getSupportText(VAR_DEF::O_FILE);
	result += getSupportText(VAR_DEF::EMPTY_STRING);
	result += emptyLine();

	result += getSupportText(FUNCT_DEF::ROUND);
	result += emptyLine();
	result += getSupportText(FUNCT_DEF::RT);
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
	result += getSupportText(FUNCT_DEF::TO_STRING);
	result += getSupportText(FUNCT_DEF::TO_ASCII);
	result += getSupportText(FUNCT_DEF::PARSE_DOUBLE);
	result += getSupportText(FUNCT_DEF::ASCII_AT);
	result += getSupportText(FUNCT_DEF::LENGTH);
	result += getSupportText(FUNCT_DEF::RETRIEVE_DOUBLE);
	result += getSupportText(FUNCT_DEF::RETRIEVE_STRING);
	result += getSupportText(FUNCT_DEF::RANDOM);
	result += getSupportText(FUNCT_DEF::READ);
	result += getSupportText(FUNCT_DEF::ELEMENT_AT);
	result += getSupportText(FUNCT_DEF::SIZE);
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
	result += getSupportText(FUNCT_DEF::DISPLAY_DBL);
	result += getSupportText(FUNCT_DEF::DISPLAY_STR);
	result += getSupportText(FUNCT_DEF::OPEN_I);
	result += getSupportText(FUNCT_DEF::OPEN_O);
	result += getSupportText(FUNCT_DEF::WRITE);
	result += getSupportText(FUNCT_DEF::CLOSE_I);
	result += getSupportText(FUNCT_DEF::CLOSE_O);
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
	result += getSupportText(STATIC_SUPPORT::MAIN_DEC);
	result += openBraceLine();

	result += getSupportText(INIT_STMT::ARGS);
	result += getSupportText(INIT_STMT::RANDOM);
	result += getSupportText(INIT_STMT::I_FILE);
	result += getSupportText(INIT_STMT::O_FILE);

	result += emptyLine();
	result += line(PEFName + "();");
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
		if (s->type == DT_DOUBLE)
			result += line(DATATYPE_TEXT::DOUBLE.getText() + " _" + s->id + " = 0;");
		else if (s->type == DT_STRING)
			result += line(DATATYPE_TEXT::STRING.getText() + " _" + s->id + " = \"\";");
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
		result += line(text.getText());
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
	return getTypeString(returnType) + " " + udfName + "(" + getArgsString(args) + ")";
}

// ----------------------------------------------------------
// This function returns the output text corresponding to
// the given DataType.
//
// Version 4.0
// ----------------------------------------------------------
string CPPLanguage::getTypeString(DataType dt)
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