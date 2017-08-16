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
class Language;

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 5.0
// ----------------------------------------------------------
struct UDFRecord : public Routine
{
	ProgramNode * root;
	SymbolTable * visibleSyms;
	CommandTable * visibleCmds;
	FunctionTable * visibleFuncts;

	UDFRecord(DataType * primary, string name, DataType * returnType, Language* lang) : Routine(primary, name, returnType, false),
	visibleCmds(new CommandTable(lang)), visibleFuncts(new FunctionTable(lang)), visibleSyms(new SymbolTable())
	{
		visibleCmds->addBuiltInVisibleCommands();
		visibleFuncts->addBuiltInVisibleFunctions();
	}
};

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function Collection.
//
// Version 5.0
// ----------------------------------------------------------
struct UDFCollection : public vector<UDFRecord *> {};

// ----------------------------------------------------------
// This class represents the data known about an Object's 
// data member
//
// Version 5.0
// ----------------------------------------------------------
struct DataRecord
{
	string memberName;
	DataType * type;
	string defaultValue;
};

// ----------------------------------------------------------
// This class represents the data known about an Object's 
// data collection.
//
// Version 5.0
// ----------------------------------------------------------
struct DataStruct : vector<DataRecord *> {};

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
	DataStruct * data;

	SymbolTable * scopedSymbols; //Symbols accessible by <obj>:id
	CommandTable * scopedCmds; //Commands accessible by <obj>:id()
	FunctionTable * scopedFuncts; //Functions accessible by <obj>:id()

	bool isUserDefined;

	ObjectStruct(Language * lang) : UDFs(new UDFCollection()), OFs(new OFCollection()), 
		data(new DataStruct()),
		scopedSymbols(new SymbolTable()), scopedCmds(new CommandTable()),
		scopedFuncts(new FunctionTable()), isUserDefined(true) {}

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