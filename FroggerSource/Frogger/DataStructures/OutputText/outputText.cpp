//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program represents output text.
// -----------------------------------------------------------------
#include "outputText.h"
using namespace std;

DATATYPE_TEXT DATATYPE_TEXT::DOUBLE		= DATATYPE_TEXT("double");
DATATYPE_TEXT DATATYPE_TEXT::STRING		= DATATYPE_TEXT("string");
DATATYPE_TEXT DATATYPE_TEXT::VOID		= DATATYPE_TEXT("void");

SYMBOL_TEXT SYMBOL_TEXT::ARGS			= SYMBOL_TEXT("args");
SYMBOL_TEXT SYMBOL_TEXT::I_FILE			= SYMBOL_TEXT("in_file");
SYMBOL_TEXT SYMBOL_TEXT::O_FILE			= SYMBOL_TEXT("out_file");
SYMBOL_TEXT SYMBOL_TEXT::EMPTY_STRING	= SYMBOL_TEXT("emptyString");

FUNCT_NAME FUNCT_NAME::TO_STRING		= FUNCT_NAME("toString");  
FUNCT_NAME FUNCT_NAME::TO_ASCII			= FUNCT_NAME("toAscii"); 
FUNCT_NAME FUNCT_NAME::PARSE_DOUBLE		= FUNCT_NAME("parseDouble"); 
FUNCT_NAME FUNCT_NAME::ASCII_AT			= FUNCT_NAME("asciiAt"); 
FUNCT_NAME FUNCT_NAME::LENGTH			= FUNCT_NAME("length"); 
FUNCT_NAME FUNCT_NAME::RETRIEVE_DOUBLE	= FUNCT_NAME("retrieveDouble"); 
FUNCT_NAME FUNCT_NAME::RANDOM			= FUNCT_NAME("random"); 
FUNCT_NAME FUNCT_NAME::RETRIEVE_STRING	= FUNCT_NAME("retrieveString"); 
FUNCT_NAME FUNCT_NAME::READ				= FUNCT_NAME("read"); 
FUNCT_NAME FUNCT_NAME::ELEMENT_AT		= FUNCT_NAME("elementAt"); 
FUNCT_NAME FUNCT_NAME::SIZE				= FUNCT_NAME("size"); 

CMD_NAME CMD_NAME::END_NULL		= CMD_NAME("end");
CMD_NAME CMD_NAME::END_STR		= CMD_NAME("end");
CMD_NAME CMD_NAME::END_DBL		= CMD_NAME("end");
CMD_NAME CMD_NAME::DISPLAY_STR	= CMD_NAME("display");
CMD_NAME CMD_NAME::DISPLAY_DBL	= CMD_NAME("display");
CMD_NAME CMD_NAME::OPEN_INPUT	= CMD_NAME("openI");
CMD_NAME CMD_NAME::CLOSE_INPUT	= CMD_NAME("closeI");
CMD_NAME CMD_NAME::WRITE		= CMD_NAME("write");
CMD_NAME CMD_NAME::OPEN_OUTPUT	= CMD_NAME("openO");
CMD_NAME CMD_NAME::CLOSE_OUTPUT	= CMD_NAME("closeO");

IMPORT_STMT IMPORT_STMT::IO_STREAM	= IMPORT_STMT("#include <iostream>");
IMPORT_STMT IMPORT_STMT::MATH		= IMPORT_STMT("#include <math.h>");
IMPORT_STMT IMPORT_STMT::STD_LIB	= IMPORT_STMT("#include <stdlib.h>");
IMPORT_STMT IMPORT_STMT::TIME		= IMPORT_STMT("#include <time.h>");
IMPORT_STMT IMPORT_STMT::F_STREAM	= IMPORT_STMT("#include <fstream>");
IMPORT_STMT IMPORT_STMT::STRING		= IMPORT_STMT("#include <string>");
IMPORT_STMT IMPORT_STMT::VECTOR		= IMPORT_STMT("#include <vector>");

VAR_DEF VAR_DEF::ARGS				= VAR_DEF("vector<string> " + SYMBOL_TEXT::ARGS.getText() + ";");
VAR_DEF VAR_DEF::I_FILE				= VAR_DEF("ifstream " + SYMBOL_TEXT::I_FILE.getText() + ";");
VAR_DEF VAR_DEF::O_FILE				= VAR_DEF("ofstream " + SYMBOL_TEXT::O_FILE.getText() + ";");
VAR_DEF VAR_DEF::EMPTY_STRING		= VAR_DEF("const string " + SYMBOL_TEXT::EMPTY_STRING.getText() + " = \"\";");

FUNCT_DEF FUNCT_DEF::ROUND				= FUNCT_DEF("double round(double num) {\n\treturn (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);\n}");
FUNCT_DEF FUNCT_DEF::RT					= FUNCT_DEF("double rt(double l, double r) {\n\t return pow(r, 1.0 / l);\n}");
FUNCT_DEF FUNCT_DEF::TO_STRING			= FUNCT_DEF("string " + FUNCT_NAME::TO_STRING.getText() + "(double d) { return to_string(d); }");
FUNCT_DEF FUNCT_DEF::TO_ASCII			= FUNCT_DEF("char " + FUNCT_NAME::TO_ASCII.getText() + "(double d) { return (char) d; }");
FUNCT_DEF FUNCT_DEF::PARSE_DOUBLE		= FUNCT_DEF("double " + FUNCT_NAME::PARSE_DOUBLE.getText() + "(string s)\n{\n\tif (isdigit(s[0]) || s[0] == '-')\n\t{\n\t\treturn stod(s, NULL);\n\t}\n\treturn 0;\n}");
FUNCT_DEF FUNCT_DEF::ASCII_AT			= FUNCT_DEF("double " + FUNCT_NAME::ASCII_AT.getText() + "(string s, int loc)\n{\n\tif (loc < 0 || loc >= s.length())\n\t{\n\t\treturn 0;\n\t}\n\treturn s.at(loc);\n}");
FUNCT_DEF FUNCT_DEF::LENGTH				= FUNCT_DEF("double " + FUNCT_NAME::LENGTH.getText() + "(string s) { return (emptyString + s).size(); }");
FUNCT_DEF FUNCT_DEF::RETRIEVE_DOUBLE	= FUNCT_DEF("double " + FUNCT_NAME::RETRIEVE_DOUBLE.getText() + "()\n{\n\tdouble d = 0;\n\tcin >> d;\n\treturn d;\n}");
FUNCT_DEF FUNCT_DEF::RETRIEVE_STRING	= FUNCT_DEF("string " + FUNCT_NAME::RETRIEVE_STRING.getText() + "()\n{\n\tstring s = "";\n\tcin >> s;\n\treturn s;\n}");
FUNCT_DEF FUNCT_DEF::RANDOM				= FUNCT_DEF("double " + FUNCT_NAME::RANDOM.getText() + "() { return ((double) rand() / (RAND_MAX)); }");
FUNCT_DEF FUNCT_DEF::READ				= FUNCT_DEF("char " + FUNCT_NAME::READ.getText() + "() { return (char)(in_file.get()); }");
FUNCT_DEF FUNCT_DEF::ELEMENT_AT			= FUNCT_DEF("string " + FUNCT_NAME::ELEMENT_AT.getText() + "(vector<string> v, int index)\n{\n\tif (index < 0 || index >= v.size())\n\t{\n\t\treturn \"\";\n\t}\n\treturn v[index];\n}");
FUNCT_DEF FUNCT_DEF::SIZE				= FUNCT_DEF("double " + FUNCT_NAME::SIZE.getText() + "(vector<string> v) { return v.size(); }");
FUNCT_DEF FUNCT_DEF::DISPLAY_DBL		= FUNCT_DEF("void " + CMD_NAME::DISPLAY_DBL.getText() + "(double d) { cout << d; }");
FUNCT_DEF FUNCT_DEF::DISPLAY_STR		= FUNCT_DEF("void " + CMD_NAME::DISPLAY_STR.getText() + "(string s) { cout << s; }");
FUNCT_DEF FUNCT_DEF::OPEN_I				= FUNCT_DEF("void " + CMD_NAME::OPEN_INPUT.getText() + "(string s) { " + SYMBOL_TEXT::I_FILE.getText() + ".open(s); }");
FUNCT_DEF FUNCT_DEF::OPEN_O				= FUNCT_DEF("void " + CMD_NAME::OPEN_OUTPUT.getText() + "(string s) { " + SYMBOL_TEXT::O_FILE.getText() + ".open(s); }");
FUNCT_DEF FUNCT_DEF::WRITE				= FUNCT_DEF("void " + CMD_NAME::WRITE.getText() + "(string s) { " + SYMBOL_TEXT::O_FILE.getText() + " << s; }");
FUNCT_DEF FUNCT_DEF::CLOSE_I			= FUNCT_DEF("void " + CMD_NAME::CLOSE_INPUT.getText() + "() { " + SYMBOL_TEXT::I_FILE.getText() + ".close(); }");
FUNCT_DEF FUNCT_DEF::CLOSE_O			= FUNCT_DEF("void " + CMD_NAME::CLOSE_OUTPUT.getText() + "() { " + SYMBOL_TEXT::O_FILE.getText() + ".close(); }");

INIT_STMT INIT_STMT::ARGS			= INIT_STMT(SYMBOL_TEXT::ARGS.getText() + " = vector<string>(argv + 1, argv + argc);");
INIT_STMT INIT_STMT::RANDOM			= INIT_STMT("srand(time(NULL)); rand();");
INIT_STMT INIT_STMT::I_FILE			= INIT_STMT(SYMBOL_TEXT::I_FILE.getText() + " = ifstream();");
INIT_STMT INIT_STMT::O_FILE			= INIT_STMT(SYMBOL_TEXT::O_FILE.getText() + " = ofstream();");

STATIC_SUPPORT STATIC_SUPPORT::USING		= STATIC_SUPPORT("using namespace std;");
STATIC_SUPPORT STATIC_SUPPORT::MAIN_DEC		= STATIC_SUPPORT("int main(int argc, char* argv[])");