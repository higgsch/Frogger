// -----------------------------------------------------------------
// This is the header for the Language class.
// -----------------------------------------------------------------
#pragma once

#include "..\Tables\tables.h"
#include <vector>
using namespace std;

// set of Output types
typedef enum output_types {
	OUTTYPE_DATATYPE, OUTTYPE_SYMBOL, OUTTYPE_SUPPORTCODE, OUTTYPE_UNINIT
} output_type;

// ----------------------------------------------------------
// This class represents a generic piece of text for output
//
// Version 4.2
// ----------------------------------------------------------
class OUTPUT_TEXT
{
private:
	string text;
	output_type type;

public:
	OUTPUT_TEXT(string text, output_type type) : text(text), type(type) {}
	OUTPUT_TEXT() : text(""), type(OUTTYPE_UNINIT) {}

	string getText() { return text; }
	output_type getOutputType() { return type; }
};

// ----------------------------------------------------------
// This class represents a datatype for output
//
// Version 4.2
// ----------------------------------------------------------
class DATATYPE_TEXT : public OUTPUT_TEXT
{
public:
	DATATYPE_TEXT(string text) : OUTPUT_TEXT(text, OUTTYPE_DATATYPE) {}
	DATATYPE_TEXT() {}
};

// ----------------------------------------------------------
// This class represents a variable name for output
//
// Version 4.2
// ----------------------------------------------------------
class SYMBOL_TEXT : public OUTPUT_TEXT
{
public:
	SYMBOL_TEXT(string text) : OUTPUT_TEXT(text, OUTTYPE_SYMBOL) {}
	SYMBOL_TEXT() {}
};

// ----------------------------------------------------------
// This class represents a function name for output
//
// Version 4.2
// ----------------------------------------------------------
class FUNCT_NAME : public OUTPUT_TEXT
{
public:
	FUNCT_NAME(string text) : OUTPUT_TEXT(text, OUTTYPE_SYMBOL) {}
	FUNCT_NAME() {}
};

// ----------------------------------------------------------
// This class represents a command name for output
//
// Version 4.2
// ----------------------------------------------------------
class CMD_NAME : public OUTPUT_TEXT
{
public:
	CMD_NAME(string text) : OUTPUT_TEXT(text, OUTTYPE_SYMBOL) {}
	CMD_NAME() {}
};

// ----------------------------------------------------------
// This class represents a generic Language package.
//
// Version 4.2
// ----------------------------------------------------------
class Language
{
private:

	Language() : builtInCommands(new CommandTable(this)), builtInFunctions(new FunctionTable(this)), builtInSymbols(new SymbolTable(this)) {}

	string indentString; // the string to use for indenting
	int indentDepth; // the number of indentChars to insert

protected:
	virtual void initOutputTexts() =0;
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
	Language(string indentString) : indentString(indentString), indentDepth(0)
	{ 
		/*
		initOutputTexts();
		builtInCommands = new CommandTable(this);
		builtInFunctions = new FunctionTable(this);
		builtInSymbols = new SymbolTable(this);*/
	}

	CommandTable* builtInCommands;
	FunctionTable* builtInFunctions;
	SymbolTable* builtInSymbols;

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

	//Support Requirements Phase Accessors
	virtual void argsUsed() =0;
	virtual void toStringUsed() =0;
	virtual void toAsciiUsed() =0;
	virtual void parseDoubleUsed() =0;
	virtual void asciiAtUsed() =0;
	virtual void lengthUsed() =0;
	virtual void retrieveDoubleUsed() =0;
	virtual void retrieveStringUsed() =0;
	virtual void randomUsed() =0;
	virtual void readUsed() =0;
	virtual void elementAtUsed() =0;
	virtual void sizeUsed() =0;
	virtual void displayDoubleUsed() =0;
	virtual void displayStringUsed() =0;
	virtual void openInputUsed() =0;
	virtual void closeInputUsed() =0;
	virtual void openOutputUsed() =0;
	virtual void writeUsed() =0;
	virtual void closeOutputUsed() =0;
	virtual void addOpUsed() =0;
	virtual void modDivOpUsed() =0;
	virtual void iDivOpUsed() =0;
	virtual void rootOpUsed() =0;
	virtual void expOpUsed() =0;
	
	//Output Texts
	DATATYPE_TEXT DT_DOUBLE;
	DATATYPE_TEXT DT_STRING;
	DATATYPE_TEXT DT_VOID;

	SYMBOL_TEXT SYM_ARGS;
	SYMBOL_TEXT SYM_I_FILE;
	SYMBOL_TEXT SYM_O_FILE;

	FUNCT_NAME FUNCTNAME_TO_STRING;
	FUNCT_NAME FUNCTNAME_TO_ASCII;
	FUNCT_NAME FUNCTNAME_PARSE_DOUBLE;
	FUNCT_NAME FUNCTNAME_ASCII_AT;
	FUNCT_NAME FUNCTNAME_LENGTH;
	FUNCT_NAME FUNCTNAME_RETRIEVE_DOUBLE;
	FUNCT_NAME FUNCTNAME_RANDOM;
	FUNCT_NAME FUNCTNAME_RETRIEVE_STRING;
	FUNCT_NAME FUNCTNAME_READ;
	FUNCT_NAME FUNCTNAME_ELEMENT_AT;
	FUNCT_NAME FUNCTNAME_SIZE;
	
	CMD_NAME CMDNAME_END_NULL;
	CMD_NAME CMDNAME_END_STR;
	CMD_NAME CMDNAME_END_DBL;
	CMD_NAME CMDNAME_DISPLAY_STR;
	CMD_NAME CMDNAME_DISPLAY_DBL;
	CMD_NAME CMDNAME_OPEN_INPUT;
	CMD_NAME CMDNAME_CLOSE_INPUT;
	CMD_NAME CMDNAME_WRITE;
	CMD_NAME CMDNAME_OPEN_OUTPUT;
	CMD_NAME CMDNAME_CLOSE_OUTPUT;
};