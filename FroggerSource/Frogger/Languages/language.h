// -----------------------------------------------------------------
// This is the header for the Language class.
// -----------------------------------------------------------------
#pragma once

#include "..\DataStructures\Tables\tables.h"
#include "..\DataStructures\Nodes\AsciiNodes\BinaryNodes\BinaryOpNodes\binaryOpNode.h"
#include <vector>
using namespace std;

struct ProgramStruct;

// ----------------------------------------------------------
// This class represents a generic piece of text for output
//
// Version 4.2
// ----------------------------------------------------------
class OUTPUT_TEXT : public string
{
public:
	OUTPUT_TEXT(string text) : string(text) {}
	OUTPUT_TEXT() : string("") {}
};

// ----------------------------------------------------------
// This class represents a generic Language package.
//
// Version 4.3
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
	Language(string indentString) : indentString(indentString), indentDepth(0) {}

	void initialize()
	{ 
		initOutputTexts();
		builtInCommands = new CommandTable(this);
		builtInFunctions = new FunctionTable(this);
		builtInSymbols = new SymbolTable(this);
	}

	virtual void outputToExe(string outFilename, string exeFilename) =0;
	virtual void cleanup(string filename) =0;

	CommandTable* builtInCommands;
	FunctionTable* builtInFunctions;
	SymbolTable* builtInSymbols;

	virtual string getMetaCode(ProgramStruct * structure) =0;
	virtual string getPEFCode(UDFRecord* rec, string pefText) =0;
	virtual string getUDFCode(UDFRecord* rec, string udfText) =0;

	virtual string getJmpStmtText(string udfName, int stmtNo, int jmpNo, bool isOwnLine, string stmtText) =0;
	virtual string getIfStmtText(string udfName, int stmtNo, bool isOwnLine,
								string boolExpText, string trueStmtText, string falseStmtText) =0;

	virtual string getAssignmentText(string assigneeText, string assignorText) =0;

	//Routines
	virtual string getFunctionCallText(bool isBuiltIn, string primaryText, string name, string argListText) =0;
	virtual string getCommandCallText(bool isBuiltIn, string primaryText, string name, string argListText) =0;
	virtual string getArgumentListText(string thisArgText, string argTailText) =0;

	virtual string getBinaryOpText(bool isNested, bool isString, string leftOperandText, string rightOperandText, binaryOp op) =0;
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
	OUTPUT_TEXT DT_DOUBLE;
	OUTPUT_TEXT DT_STRING;
	OUTPUT_TEXT DT_VOID;

	OUTPUT_TEXT SYM_ARGS;
	OUTPUT_TEXT SYM_I_FILE;
	OUTPUT_TEXT SYM_O_FILE;

	OUTPUT_TEXT FUNCTNAME_TO_STRING;
	OUTPUT_TEXT FUNCTNAME_TO_ASCII;
	OUTPUT_TEXT FUNCTNAME_PARSE_DOUBLE;
	OUTPUT_TEXT FUNCTNAME_ASCII_AT;
	OUTPUT_TEXT FUNCTNAME_LENGTH;
	OUTPUT_TEXT FUNCTNAME_RETRIEVE_DOUBLE;
	OUTPUT_TEXT FUNCTNAME_RANDOM;
	OUTPUT_TEXT FUNCTNAME_RETRIEVE_STRING;
	OUTPUT_TEXT FUNCTNAME_READ;
	OUTPUT_TEXT FUNCTNAME_ELEMENT_AT;
	OUTPUT_TEXT FUNCTNAME_SIZE;
	
	OUTPUT_TEXT CMDNAME_END_NULL;
	OUTPUT_TEXT CMDNAME_END_STR;
	OUTPUT_TEXT CMDNAME_END_DBL;
	OUTPUT_TEXT CMDNAME_DISPLAY_STR;
	OUTPUT_TEXT CMDNAME_DISPLAY_DBL;
	OUTPUT_TEXT CMDNAME_OPEN_INPUT;
	OUTPUT_TEXT CMDNAME_CLOSE_INPUT;
	OUTPUT_TEXT CMDNAME_WRITE;
	OUTPUT_TEXT CMDNAME_OPEN_OUTPUT;
	OUTPUT_TEXT CMDNAME_CLOSE_OUTPUT;
};