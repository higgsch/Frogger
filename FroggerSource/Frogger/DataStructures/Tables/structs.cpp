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
ProgramStruct::ProgramStruct(Language * lang) : ObjectStruct(lang) 
{
	OBJ_DOUBLE = new ObjectStruct(lang);
	OBJ_STRING = new ObjectStruct(lang);
	OBJ_STRING_LIST = new ObjectStruct(lang);

	OBJ_DOUBLE->name = DataType::DT_DOUBLE->typeString;
		OBJ_DOUBLE->isUserDefined = false;
		OBJ_DOUBLE->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_TO_STRING));
		OBJ_DOUBLE->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_TO_STRING));

		OBJ_DOUBLE->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_TO_ASCII));
		OBJ_DOUBLE->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_TO_ASCII));

	OBJ_STRING->name = DataType::DT_STRING->typeString;
		OBJ_STRING->isUserDefined = false;
		OBJ_STRING->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_ASCII_AT));
		OBJ_STRING->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_ASCII_AT));

		OBJ_STRING->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_LENGTH));
		OBJ_STRING->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_LENGTH));

		OBJ_STRING->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_PARSE_DOUBLE));
		OBJ_STRING->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_PARSE_DOUBLE));


	OBJ_STRING_LIST->name = DataType::DT_STRINGLIST->typeString;
		OBJ_STRING_LIST->isUserDefined = false;
		OBJ_STRING_LIST->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_ELEMENT_AT));
		OBJ_STRING_LIST->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_ELEMENT_AT));

		OBJ_STRING_LIST->visibleFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_SIZE));
		OBJ_STRING_LIST->scopedFuncts->add(new FunctionRecord(lang->builtInFunctions->FUNCT_SIZE));

	OFs->push_back(OBJ_DOUBLE);
	OFs->push_back(OBJ_STRING);
	OFs->push_back(OBJ_STRING_LIST);
}