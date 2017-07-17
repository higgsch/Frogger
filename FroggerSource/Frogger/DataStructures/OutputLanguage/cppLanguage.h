// -----------------------------------------------------------------
// This is the header for the CPPLanguage class.
// -----------------------------------------------------------------
#pragma once

#include "language.h"
#include "..\OutputText\outputText.h"
using namespace std;

// ----------------------------------------------------------
// This class represents the CPPLanguage package.
//
// Version 4.2
// ----------------------------------------------------------
class CPPLanguage : public Language
{
private:
	//Constants for C++ symbols
	static const string OPEN_BRACE;
	static const string CLOSE_BRACE;
	static const string OPEN_NEST;
	static const string CLOSE_NEST;
	static const string INDENT;

	string getUsingStatementText();
	string getSupportCode();
	string getBuiltInFunctionCode();
	string getBuiltInCommandCode();
	string getForwardDeclarationCode(ProgramStruct * prog);
	string getMainFunctionText(string PEFName);

	string getSymbolTableCode(SymbolTable * symbols);
	string getSupportText(SUPPORT_TEXT& text);

	string getLabelText(string udfName, int stmtNo);
	string getFunctionPrototype(DataType returnType, string udfName, vector<argPair*>* args);
	string getTypeString(DataType type);
	string getArgsString(vector<argPair*>* args);

	// ----------------------------------------------------------
	// This function returns a formatted line with an opening brace
	//
	// Version 4.2
	// ----------------------------------------------------------
	string openBraceLine() 
	{
		string result = line(OPEN_BRACE);
		increaseIndent();
		return result;
	}
	// ----------------------------------------------------------
	// This function returns a formatted line with a closing brace
	//
	// Version 4.2
	// ----------------------------------------------------------
	string closeBraceLine()
	{
		decreaseIndent();
		return line(CLOSE_BRACE);
	}

	// ----------------------------------------------------------
	// This function returns the given string nested within the 
	// nesting constants if nested, the given string un-changed
	// otherwise.
	// @nested: a flag to drive nesting
	// @toNest: the string to nest
	//
	// Version 4.2
	// ----------------------------------------------------------
	string nest(bool nested, string toNest)	{ return (nested) ? OPEN_NEST + toNest + CLOSE_NEST : toNest; }
	
	string getBinaryOperationText(bool nested, string pretext, string midtext, string posttext, 
										   string leftOperandText, string rightOperandText);

public:
	CPPLanguage() : Language(INDENT) {}

	//Highest Level
	string getMetaCode(ProgramStruct * structure);
	string getPEFCode(UDFRecord* rec, SymbolTable* symbols, string pefText);
	string getUDFCode(UDFRecord* rec, SymbolTable* symbols, string udfText);
	
	//Stmt
	string getJmpStmtText(string udfName, int stmtNo, int jmpNo, bool isOwnLine, string stmtText);
	string getIfStmtText(string udfName, int stmtNo, bool isOwnLine,
						string boolExpText, string trueStmtText, string falseStmtText);

	//Assignment
	string getAssignmentText(string assigneeText, string assignorText);

	//Routines
	string getFunctionCallText(bool isBuiltIn, string primaryText, string name, string argListText);
	string getCommandCallText(bool isBuiltIn, string primaryText, string name, string argListText);
	string getArgumentListText(string thisArgText, string argTailText);

	//Operations
	string getAddOperationText(bool isNested, bool isString, string leftOperandText, string rightOperandText);
	string getSubOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getMulOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getDivOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getModDivOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getIDivOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getRootOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getExpOperationText(bool isNested, string leftOperandText, string rightOperandText);

	//Boolean Comparisons
	string getLTOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getGTOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getEQOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getLTEOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getGTEOperationText(bool isNested, string leftOperandText, string rightOperandText);
	string getNotOperationText(string notText);

	//Terminals
	string getIdentifierText(bool nested, string id);
	string getStringLiteralText(string str);
	string getDoubleLiteralText(bool isNested, string dbl);
};