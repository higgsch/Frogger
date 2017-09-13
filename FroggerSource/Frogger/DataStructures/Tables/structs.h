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
struct ObjectStruct;

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 5.2
// ----------------------------------------------------------
struct UDFRecord : public Routine
{
	string filepath;
	ProgramNode * root;
	TableGroup * visibleTables;
	ObjectStruct * containingOF;

	UDFRecord(DataType * primary, string name, DataType * returnType, Language* lang) : Routine(primary, name, returnType, false),
		visibleTables(new TableGroup(lang)), filepath(""), containingOF(NULL)
	{
		visibleTables->cmds->addBuiltInVisibleCommands();
		visibleTables->functs->addBuiltInVisibleFunctions();
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
struct DataCollection : vector<DataRecord *> {};

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
// This class represents the data known about a OF's Templatization.
//
// Version 5.2
// ----------------------------------------------------------
struct TemplatizationCollection : vector<string> 
{
public:
	bool contains(string templateID)
	{
		int templateCount = size();
		for (int templateIndex = 0; templateIndex < templateCount; templateIndex++)
		{
			if (templateID == at(templateIndex))
				return true;
		}
		return false;
	}

	int getIndex(string templateID)
	{
		int templateCount = size();
		for (int templateIndex = 0; templateIndex < templateCount; templateIndex++)
		{
			if (templateID == at(templateIndex))
				return templateIndex;
		}
		return -1;
	}
};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Object. Generated from the .struct file.
//
// Version 5.2
// ----------------------------------------------------------
struct ObjectStruct
{
	string name;
	string parentName;
	ObjectStruct * parent;
	UDFCollection * UDFs;
	OFCollection * OFs;
	DataCollection * data;
	TemplatizationCollection * templatizationList;

	TableGroup * scopedTables; //Records accessible by scoping (e.g. <obj>:id())

	bool isUserDefined;
	bool isCodeGenerated;
	
	DataTypeCollection * types;
	DataType * type;
	DataTypeCollection * templatizedTypes;

	ObjectStruct() : UDFs(new UDFCollection()), OFs(new OFCollection()), 
		data(new DataCollection()), templatizationList(new TemplatizationCollection()),
		scopedTables(new TableGroup()), isUserDefined(true), isCodeGenerated(false),
		parentName(""), parent(NULL) {}

	bool isTemplatized() { return templatizationList->size() != 0; }
	bool hasParent() { return parent != NULL; }
	int getNumberOfUDFs() { return UDFs->size(); }
	int getNumberOfOFs() { return OFs->size(); }
	UDFRecord * getUDF(int udfIndex) { return (*UDFs)[udfIndex]; }
	ProgramNode * getUDFNode(int udfIndex) { return (*UDFs)[udfIndex]->root; }
	ObjectStruct * getOF(int objIndex) { return (*OFs)[objIndex]; }

	ObjectStruct * getObject(string remainingObjectName)
	{
		string objectName = "";
		size_t scopeOperatorPos = remainingObjectName.find_first_of(":");
		if (scopeOperatorPos == string::npos)
			objectName = remainingObjectName;
		else
			objectName = remainingObjectName.substr(0, scopeOperatorPos);

		int objCount = OFs->size();
		for (int objIndex = 0; objIndex < objCount; objIndex++)
		{
			ObjectStruct * currObj = getOF(objIndex);
			if (objectName == currObj->name)
			{
				if (scopeOperatorPos == string::npos)
					return currObj;
				else
					return currObj->getObject(remainingObjectName.substr(scopeOperatorPos + 1));
			}
		}

		return NULL;
	}

	UDFRecord* operator[](int index) { return (*UDFs)[index]; }
};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Program. Generated from the .struct file.
//
// Version 5.2
// ----------------------------------------------------------
struct ProgramStruct : public ObjectStruct
{
	UDFRecord * PEF;

	ProgramStruct(Language * lang);

	ProgramNode* getPEFNode() { return PEF->root; }

	ObjectStruct * getObject(DataType * dt)
	{
		string objectName = dt->fullyScopedTypeString();
		return getObject(objectName);
	}

	ObjectStruct * getObject(string objectName)
	{
		return ((ObjectStruct*)this)->getObject(objectName);
	}

	ObjectStruct * OBJ_DOUBLE;
	ObjectStruct * OBJ_STRING;
	ObjectStruct * OBJ_STRING_LIST;
};