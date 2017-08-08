// -----------------------------------------------------------------
// This is the header for AST structures
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "records.h"
#include "tables.h"
using namespace std;

//forward declarations
class ProgramNode;

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 5.0
// ----------------------------------------------------------
struct UDFRecord : public Routine
{
	ProgramNode * root;
	SymbolTable * symbols;

	UDFRecord(DataType * primary, string name, DataType * returnType) : Routine(primary, name, returnType, false) {}
};

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function Collection.
//
// Version 5.0
// ----------------------------------------------------------
struct UDFCollection : public vector<UDFRecord *> {};

//Forward declaration
struct ObjectStruct;
class Language;

// ----------------------------------------------------------
// This class represents the data known about an Object File
// Collection.
//
// Version 5.0
// ----------------------------------------------------------
struct OFCollection : vector<ObjectStruct *> {};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Object. Generated from the .struct file.
//
// Version 5.0
// ----------------------------------------------------------
struct ObjectStruct
{
	string name;
	UDFCollection * UDFs;
	OFCollection * OFs;

	SymbolTable * symbols;
	CommandTable * cmds;
	FunctionTable * functs;

	ObjectStruct(Language * lang) : UDFs(new UDFCollection()), OFs(new OFCollection()), 
		symbols(new SymbolTable(lang)), cmds(new CommandTable(lang)), functs(new FunctionTable(lang)) {}

	int getNumberOfUDFs() { return UDFs->size(); }
	int getNumberOfOFs() { return OFs->size(); }
	UDFRecord * getUDF(int udfIndex) { return (*UDFs)[udfIndex]; }
	ProgramNode * getUDFNode(int udfIndex) { return (*UDFs)[udfIndex]->root; }
	ObjectStruct * getOF(int objIndex) { return (*OFs)[objIndex]; }

	UDFRecord* operator[](int index) { return (*UDFs)[index]; }
};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Program. Generated from the .struct file.
//
// Version 5.0
// ----------------------------------------------------------
struct ProgramStruct : public ObjectStruct
{
	UDFRecord * PEF;

	ProgramStruct(Language * lang) : ObjectStruct(lang) {}

	ProgramNode* getPEFNode() { return PEF->root; }
};