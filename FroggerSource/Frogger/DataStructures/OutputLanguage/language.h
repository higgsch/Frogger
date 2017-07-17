// -----------------------------------------------------------------
// This is the header for the Language class.
// -----------------------------------------------------------------
#pragma once

#include "..\Tables\tables.h"
using namespace std;

// ----------------------------------------------------------
// This class represents a generic Language package.
//
// Version 4.2
// ----------------------------------------------------------
class Language
{
private:

	Language(){}

	string indentString; // the string to use for indenting
	int indentDepth; // the number of indentChars to insert

protected:
	virtual string getBinaryOperationText(bool nested, string pretext, string midtext, string posttext, 
										   string leftOperandText, string rightOperandText) =0;
	void increaseIndent() { indentDepth++; }
	void decreaseIndent() { indentDepth--; }

	string emptyLine() { return "\n"; }
	string line(string s) { return indent() + s + "\n"; }
	string indent()
	{
		string result = "";
		for (int i = 0; i < indentDepth; i++)
			result = result + indentString;

		return result;
	}

public:
	Language(string indentString) : indentString(indentString), indentDepth(0) {}

	virtual string getMetaCode(ProgramStruct * structure) =0;
	virtual string getPEFCode(UDFRecord* rec, SymbolTable* symbols, string pefText) =0;
	virtual string getUDFCode(UDFRecord* rec, SymbolTable* symbols, string udfText) =0;

	virtual string getJmpStmtText(string udfName, int stmtNo, int jmpNo, bool isOwnLine, string stmtText) =0;
	virtual string getIfStmtText(string udfName, int stmtNo, bool isOwnLine,
								string boolExpText, string trueStmtText, string falseStmtText) =0;

	virtual string getAssignmentText(string assigneeText, string assignorText) =0;

	//Routines
	virtual string getFunctionCallText(bool isBuiltIn, string primaryText, string name, string argListText) =0;
	virtual string getCommandCallText(bool isBuiltIn, string primaryText, string name, string argListText) =0;
	virtual string getArgumentListText(string thisArgText, string argTailText) =0;

	//Operations
	virtual string getAddOperationText(bool isNested, bool isString, string leftOperandText, string rightOperandText) =0;
	virtual string getSubOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getMulOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getDivOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getModDivOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getIDivOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getRootOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getExpOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;

	//Boolean Comparisons
	virtual string getLTOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getGTOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getEQOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getLTEOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getGTEOperationText(bool isNested, string leftOperandText, string rightOperandText) =0;
	virtual string getNotOperationText(string notText) =0;

	//Terminals
	virtual string getIdentifierText(bool nested, string id) =0;
	virtual string getStringLiteralText(string str) =0;
	virtual string getDoubleLiteralText(bool isNested, string dbl) =0;
};