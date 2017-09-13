//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the AST Structures
// -----------------------------------------------------------------
#include "structs.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// Primary Constructor: populates with built in objects
//
// Version 5.0
// ----------------------------------------------------------
ProgramStruct::ProgramStruct(Language * lang) : ObjectStruct() 
{
	OBJ_DOUBLE = new ObjectStruct();
	OBJ_STRING = new ObjectStruct();
	OBJ_STRING_LIST = new ObjectStruct();

	OBJ_DOUBLE->name = DataType::DT_DOUBLE->typeName;
		OBJ_DOUBLE->isUserDefined = false;
		OBJ_DOUBLE->scopedTables->functs->add(lang->builtInFunctions->FUNCT_TO_STRING);
		OBJ_DOUBLE->scopedTables->functs->add(lang->builtInFunctions->FUNCT_TO_ASCII);

	OBJ_STRING->name = DataType::DT_STRING->typeName;
		OBJ_STRING->isUserDefined = false;
		OBJ_STRING->scopedTables->functs->add(lang->builtInFunctions->FUNCT_ASCII_AT);
		OBJ_STRING->scopedTables->functs->add(lang->builtInFunctions->FUNCT_LENGTH);
		OBJ_STRING->scopedTables->functs->add(lang->builtInFunctions->FUNCT_PARSE_DOUBLE);


	OBJ_STRING_LIST->name = DataType::DT_STRINGLIST->typeName;
		OBJ_STRING_LIST->isUserDefined = false;
		OBJ_STRING_LIST->scopedTables->functs->add(lang->builtInFunctions->FUNCT_ELEMENT_AT);
		OBJ_STRING_LIST->scopedTables->functs->add(lang->builtInFunctions->FUNCT_SIZE);

	OFs->push_back(OBJ_DOUBLE);
	OFs->push_back(OBJ_STRING);
	OFs->push_back(OBJ_STRING_LIST);
}