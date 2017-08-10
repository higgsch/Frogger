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

	SymbolTable * scopedSymbols; //Symbols accessible by <obj>:id
	SymbolTable * visibleSymbols; //Symbols accessible within the object
	CommandTable * scopedCmds; //Commands accessible by <obj>:id()
	CommandTable * visibleCmds; //Commands accessible within the object
	FunctionTable * scopedFuncts; //Functions accessible by <obj>:id()
	FunctionTable * visibleFuncts; //Functions accessible within the object

	bool isUserDefined;

	ObjectStruct(Language * lang) : UDFs(new UDFCollection()), OFs(new OFCollection()), 
		scopedSymbols(new SymbolTable()), visibleSymbols(new SymbolTable(lang)),
		scopedCmds(new CommandTable()), visibleCmds(new CommandTable(lang)),
		scopedFuncts(new FunctionTable()), visibleFuncts(new FunctionTable(lang)),
		isUserDefined(true) {}

	int getNumberOfUDFs() { return UDFs->size(); }
	int getNumberOfOFs() { return OFs->size(); }
	UDFRecord * getUDF(int udfIndex) { return (*UDFs)[udfIndex]; }
	ProgramNode * getUDFNode(int udfIndex) { return (*UDFs)[udfIndex]->root; }
	ObjectStruct * getOF(int objIndex) { return (*OFs)[objIndex]; }

	ObjectStruct * getObject(DataType * dt)
	{
		string objectName = dt->typeString;
		int objCount = OFs->size();
		for (int objIndex = 0; objIndex < objCount; objIndex++)
		{
			ObjectStruct * currObj = getOF(objIndex);
			if (objectName == currObj->name)
				return currObj;
		}

		return NULL;
	}

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
	DataTypeCollection * types;

	ProgramStruct(Language * lang);

	ProgramNode* getPEFNode() { return PEF->root; }

	ObjectStruct * OBJ_DOUBLE;
	ObjectStruct * OBJ_STRING;
	ObjectStruct * OBJ_STRING_LIST;
};