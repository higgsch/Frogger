// -----------------------------------------------------------------
// This is the header file for OutputText classes. 
// -----------------------------------------------------------------
#pragma once

#include <string>
using namespace std;

// set of Output types
typedef enum output_types {
	OUTTYPE_KEYWORD, OUTTYPE_SYMBOL, OUTTYPE_SUPPORTCODE
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

// set of Support Code Types
typedef enum support_code_types {
	SCT_IMPORT, SCT_VARIABLE, SCT_FUNCTION
} support_code_type;

// ----------------------------------------------------------
// This class represents a supporting piece of text for output
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
// This class represents a supporting piece of text for output
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
	static IMPORT_STMT USING;
};


// ----------------------------------------------------------
// This class represents a supporting piece of text for output
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
// This class represents a supporting piece of text for output
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
	static FUNCT_DEF DISPLAY;
	static FUNCT_DEF OPEN_I;
	static FUNCT_DEF OPEN_O;
	static FUNCT_DEF WRITE;
	static FUNCT_DEF CLOSE_I;
	static FUNCT_DEF CLOSE_O;
};