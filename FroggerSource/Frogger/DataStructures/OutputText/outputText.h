// -----------------------------------------------------------------
// This is the header file for OutputText classes. 
// -----------------------------------------------------------------
#pragma once

#include <string>
using namespace std;

// set of Output types
typedef enum output_types {
	OUTTYPE_DATATYPE, OUTTYPE_SYMBOL, OUTTYPE_SUPPORTCODE
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

	static DATATYPE_TEXT DOUBLE;
	static DATATYPE_TEXT STRING;
	static DATATYPE_TEXT VOID;
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

	static SYMBOL_TEXT ARGS;
	static SYMBOL_TEXT I_FILE;
	static SYMBOL_TEXT O_FILE;
	static SYMBOL_TEXT EMPTY_STRING;
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

	static FUNCT_NAME TO_STRING;
	static FUNCT_NAME TO_ASCII;
	static FUNCT_NAME PARSE_DOUBLE;
	static FUNCT_NAME ASCII_AT;
	static FUNCT_NAME LENGTH;
	static FUNCT_NAME RETRIEVE_DOUBLE;
	static FUNCT_NAME RANDOM;
	static FUNCT_NAME RETRIEVE_STRING;
	static FUNCT_NAME READ;
	static FUNCT_NAME ELEMENT_AT;
	static FUNCT_NAME SIZE;
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

	static CMD_NAME END_NULL;
	static CMD_NAME END_STR;
	static CMD_NAME END_DBL;
	static CMD_NAME DISPLAY_STR;
	static CMD_NAME DISPLAY_DBL;
	static CMD_NAME OPEN_INPUT;
	static CMD_NAME CLOSE_INPUT;
	static CMD_NAME WRITE;
	static CMD_NAME OPEN_OUTPUT;
	static CMD_NAME CLOSE_OUTPUT;
};

// set of Support Code Types
typedef enum support_code_types {
	SCT_IMPORT, SCT_VARIABLE, SCT_FUNCTION, SCT_INIT, SCT_STATIC
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
	support_code_type type;

public:
	bool isNeeded;
	bool isDefined;

	SUPPORT_TEXT(string text, support_code_type type) 
		: OUTPUT_TEXT(text, OUTTYPE_SUPPORTCODE), 
		type(type), isNeeded(false), isDefined(false) {}

	support_code_type getSupportCodeType() { return type; }
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

	static IMPORT_STMT IO_STREAM;
	static IMPORT_STMT MATH;
	static IMPORT_STMT STD_LIB;
	static IMPORT_STMT TIME;
	static IMPORT_STMT F_STREAM;
	static IMPORT_STMT STRING;
	static IMPORT_STMT VECTOR;
};


// ----------------------------------------------------------
// This class represents a variable definition for output
//
// Version 4.2
// ----------------------------------------------------------
class VAR_DEF : public SUPPORT_TEXT
{
public:
	VAR_DEF(string text) 
		: SUPPORT_TEXT(text, SCT_FUNCTION) {}

	static VAR_DEF ARGS;
	static VAR_DEF I_FILE;
	static VAR_DEF O_FILE;
	static VAR_DEF EMPTY_STRING;
};

// ----------------------------------------------------------
// This class represents a function definition for output
//
// Version 4.2
// ----------------------------------------------------------
class FUNCT_DEF : public SUPPORT_TEXT
{
public:
	FUNCT_DEF(string text) 
		: SUPPORT_TEXT(text, SCT_FUNCTION) {}

	static FUNCT_DEF ROUND;
	static FUNCT_DEF RT;
	static FUNCT_DEF TO_STRING;
	static FUNCT_DEF TO_ASCII;
	static FUNCT_DEF PARSE_DOUBLE;
	static FUNCT_DEF ASCII_AT;
	static FUNCT_DEF LENGTH;
	static FUNCT_DEF RETRIEVE_DOUBLE;
	static FUNCT_DEF RETRIEVE_STRING;
	static FUNCT_DEF RANDOM;
	static FUNCT_DEF READ;
	static FUNCT_DEF ELEMENT_AT;
	static FUNCT_DEF SIZE;
	static FUNCT_DEF DISPLAY_DBL;
	static FUNCT_DEF DISPLAY_STR;
	static FUNCT_DEF OPEN_I;
	static FUNCT_DEF OPEN_O;
	static FUNCT_DEF WRITE;
	static FUNCT_DEF CLOSE_I;
	static FUNCT_DEF CLOSE_O;
};

// ----------------------------------------------------------
// This class represents an initialization statement for output
//
// Version 4.2
// ----------------------------------------------------------
class INIT_STMT : public SUPPORT_TEXT
{
public:
	INIT_STMT(string text) 
		: SUPPORT_TEXT(text, SCT_INIT) {}

	static INIT_STMT ARGS;
	static INIT_STMT RANDOM;
	static INIT_STMT I_FILE;
	static INIT_STMT O_FILE;
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
		: SUPPORT_TEXT(text, SCT_STATIC) { isNeeded = true; }

	static STATIC_SUPPORT USING;
	static STATIC_SUPPORT MAIN_DEC;
};