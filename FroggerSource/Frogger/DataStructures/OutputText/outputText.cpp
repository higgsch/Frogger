//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents output text.
// -----------------------------------------------------------------
#include "outputText.h"
using namespace std;

IMPORT_STMT IMPORT_STMT::IO_STREAM	= IMPORT_STMT("#include <iostream>");
IMPORT_STMT IMPORT_STMT::MATH		= IMPORT_STMT("#include <math.h>");
IMPORT_STMT IMPORT_STMT::STD_LIB	= IMPORT_STMT("#include <stdlib.h>");
IMPORT_STMT IMPORT_STMT::TIME		= IMPORT_STMT("#include <time.h>");
IMPORT_STMT IMPORT_STMT::F_STREAM	= IMPORT_STMT("#include <fstream>");
IMPORT_STMT IMPORT_STMT::STRING		= IMPORT_STMT("#include <string>");
IMPORT_STMT IMPORT_STMT::VECTOR		= IMPORT_STMT("#include <vector>");
IMPORT_STMT IMPORT_STMT::USING		= IMPORT_STMT("using namespace std;");

VAR_DEF VAR_DEF::ARGS				= VAR_DEF("vector<string> args;");
VAR_DEF VAR_DEF::I_FILE				= VAR_DEF("ifstream in_file;");
VAR_DEF VAR_DEF::O_FILE				= VAR_DEF("ofstream out_file;");
VAR_DEF VAR_DEF::EMPTY_STRING		= VAR_DEF("const string emptyString = \"\";");

FUNCT_DEF FUNCT_DEF::ROUND				= FUNCT_DEF("double round(double num) {\n\treturn (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);\n}");
FUNCT_DEF FUNCT_DEF::RT					= FUNCT_DEF("double rt(double l, double r) {\n\t return pow(r, 1.0 / l);\n}");
FUNCT_DEF FUNCT_DEF::TO_STRING			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::TO_ASCII			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::PARSE_DOUBLE		= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::ASCII_AT			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::LENGTH				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::RETRIEVE_DOUBLE	= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::RETRIEVE_STRING	= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::RANDOM				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::READ				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::ELEMENT_AT			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::SIZE				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::DISPLAY			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::OPEN_I				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::OPEN_O				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::WRITE				= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::CLOSE_I			= FUNCT_DEF("");
FUNCT_DEF FUNCT_DEF::CLOSE_O			= FUNCT_DEF("");