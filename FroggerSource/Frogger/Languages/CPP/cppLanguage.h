// -----------------------------------------------------------------
// This is the header for the CPPLanguage class.
// -----------------------------------------------------------------
#pragma once

#include "..\language.h"
using namespace std;

// set of Support Code Types
typedef enum support_code_types {
	SCT_IMPORT, SCT_VARIABLE, SCT_FUNCTION, SCT_INIT, SCT_STATIC,
	SCT_UNINIT
} support_code_type;

// ----------------------------------------------------------
// This class represents a generic supporting piece of text 
// for output
//
// Version 4.2
// ----------------------------------------------------------
class SUPPORT_TEXT : public OUTPUT_TEXT
{
private:
	class DependencyList : vector<SUPPORT_TEXT *>
	{
	public:
		void add(SUPPORT_TEXT* dep) { push_back(dep); }
		void setAllAsNeeded()
		{
			for (unsigned i = 0; i < size(); i++)
				at(i)->needed();
		}
	};

	support_code_type type;
	DependencyList dependencies;
	bool required;

public:
	bool isDefined;

	SUPPORT_TEXT(string text, support_code_type type) 
		: OUTPUT_TEXT(text), 
		type(type), required(false), isDefined(false) {}
	SUPPORT_TEXT() : type(SCT_UNINIT) {}

	support_code_type getSupportCodeType() { return type; }

	void addDependency(SUPPORT_TEXT* dep) { dependencies.add(dep); }
	void needed()
	{
		required = true;
		dependencies.setAllAsNeeded();
	}
	bool isNeeded() { return required; }
};


// ----------------------------------------------------------
// This class represents an import statement for output
//
// Version 4.2
// ----------------------------------------------------------
class IMPORT_STMT : public SUPPORT_TEXT
{
public:
	IMPORT_STMT(string text) 
		: SUPPORT_TEXT(text, SCT_IMPORT) {}
	IMPORT_STMT() {}
};

// ----------------------------------------------------------
// This class represents a variable definition for output
//
// Version 4.2
// ----------------------------------------------------------
class VAR_DEF : public SUPPORT_TEXT
{
public:
	VAR_DEF(string text) : SUPPORT_TEXT(text, SCT_VARIABLE) {}
	VAR_DEF() {}
};

// ----------------------------------------------------------
// This class represents a function definition for output
//
// Version 4.2
// ----------------------------------------------------------
class FUNCT_DEF : public SUPPORT_TEXT
{
public:
	FUNCT_DEF(string text) : SUPPORT_TEXT(text, SCT_FUNCTION) {}
	FUNCT_DEF() {}
};

// ----------------------------------------------------------
// This class represents an initialization statement for output
//
// Version 4.2
// ----------------------------------------------------------
class INIT_STMT : public SUPPORT_TEXT
{
public:
	INIT_STMT(string text) : SUPPORT_TEXT(text, SCT_INIT) {}
	INIT_STMT() {}
};

// ----------------------------------------------------------
// This class represents a piece of text that is neccessary 
// for any output
//
// Version 4.2
// ----------------------------------------------------------
class STATIC_SUPPORT : public SUPPORT_TEXT
{
public:
	STATIC_SUPPORT(string text) 
		: SUPPORT_TEXT(text, SCT_STATIC) { needed(); }
	STATIC_SUPPORT() {}
};

// ----------------------------------------------------------
// This class represents the CPPLanguage package.
//
// Version 5.0
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

	string getClassForwardDeclaration(ObjectStruct * obj);
	string getClassDefinitionCode(ProgramStruct * prog);
	string getClassDefinitionCode(ObjectStruct * obj);

	string getSymbolTableCode(SymbolTable * symbols);
	string getSupportText(SUPPORT_TEXT& text);

	string getLabelText(string udfName, int stmtNo);
	string getConstructor(ObjectStruct * obj);
	string getFunctionDeclaration(UDFRecord * udf);
	string getFunctionPrototype(UDFRecord * udf);
	string getTypeString(DataType *  type);
	string getArgsString(ArgList* args);

	// ----------------------------------------------------------
	// This function returns a formatted line with an opening brace
	//
	// Version 5.0
	// ----------------------------------------------------------
	string openBraceLine() 
	{
		return line(OPEN_BRACE);
	}
	// ----------------------------------------------------------
	// This function returns a formatted line with a closing brace
	//
	// Version 5.0
	// ----------------------------------------------------------
	string closeBraceLine()
	{
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

	void cpp_error(string msg)
	{
		cout << "Compiler Error - CPP Language Error: " << msg << endl;
	
		if (!quietMode)
		{
			cout << "Press Enter to Exit" << endl;

			getchar();
		}
		exit(0);
	}

protected:
	void initOutputTexts();
	void initDependencies();

public:
	CPPLanguage() : Language(INDENT) {}

	void outputToExe(string outFilename, string exeFilename);
	void cleanup(string filename);

	//Highest Level
	string getMetaCode(ProgramStruct * structure);
	string getPEFCode(UDFRecord* rec, string pefText);
	string getUDFCode(UDFRecord* rec, string udfText);
	
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

	string getBinaryOpText(bool isNested, bool isString, string leftOperandText, string rightOperandText, binaryOp op);
	string getNotOperationText(string notText);

	//Terminals
	string getIdentifierText(bool nested, string id);
	string getStringLiteralText(string str);
	string getDoubleLiteralText(bool isNested, string dbl);

	void argsUsed();
	void toStringUsed();
	void toAsciiUsed();
	void parseDoubleUsed();
	void asciiAtUsed();
	void lengthUsed();
	void retrieveDoubleUsed();
	void retrieveStringUsed();
	void randomUsed();
	void readUsed();
	void elementAtUsed();
	void sizeUsed();
	void displayDoubleUsed();
	void displayStringUsed();
	void openInputUsed();
	void closeInputUsed();
	void openOutputUsed();
	void writeUsed();
	void closeOutputUsed();
	void addOpUsed();
	void modDivOpUsed();
	void iDivOpUsed();
	void rootOpUsed();
	void expOpUsed();

	//Output Text
	OUTPUT_TEXT SYM_EMPTY_STRING;

	IMPORT_STMT IMP_IO_STREAM;
	IMPORT_STMT IMP_MATH;
	IMPORT_STMT IMP_STD_LIB;
	IMPORT_STMT IMP_TIME;
	IMPORT_STMT IMP_F_STREAM;
	IMPORT_STMT IMP_STRING;
	IMPORT_STMT IMP_VECTOR;

	VAR_DEF SYMDEF_ARGS;
	VAR_DEF SYMDEF_I_FILE;
	VAR_DEF SYMDEF_O_FILE;
	VAR_DEF SYMDEF_EMPTY_STRING;

	FUNCT_DEF FUNCTDEF_ROUND;
	FUNCT_DEF FUNCTDEF_RT;
	FUNCT_DEF FUNCTDEF_TO_STRING;
	FUNCT_DEF FUNCTDEF_TO_ASCII;
	FUNCT_DEF FUNCTDEF_PARSE_DOUBLE;
	FUNCT_DEF FUNCTDEF_ASCII_AT;
	FUNCT_DEF FUNCTDEF_LENGTH;
	FUNCT_DEF FUNCTDEF_RETRIEVE_DOUBLE;
	FUNCT_DEF FUNCTDEF_RETRIEVE_STRING;
	FUNCT_DEF FUNCTDEF_RANDOM;
	FUNCT_DEF FUNCTDEF_READ;
	FUNCT_DEF FUNCTDEF_ELEMENT_AT;
	FUNCT_DEF FUNCTDEF_SIZE;
	FUNCT_DEF FUNCTDEF_DISPLAY_DBL;
	FUNCT_DEF FUNCTDEF_DISPLAY_STR;
	FUNCT_DEF FUNCTDEF_OPEN_I;
	FUNCT_DEF FUNCTDEF_OPEN_O;
	FUNCT_DEF FUNCTDEF_WRITE;
	FUNCT_DEF FUNCTDEF_CLOSE_I;
	FUNCT_DEF FUNCTDEF_CLOSE_O;

	INIT_STMT INIT_ARGS;
	INIT_STMT INIT_RANDOM;
	INIT_STMT INIT_I_FILE;
	INIT_STMT INIT_O_FILE;

	STATIC_SUPPORT USING;
	STATIC_SUPPORT MAIN_DEC;
};