//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.2
// -----------------------------------------------------------------
// This program provides for data typing
// -----------------------------------------------------------------
#include "dataTyped.h"
using namespace std;

const string DataType::SCOPE_OPERATOR = ":";
DataType* DataType::DT_NULL = new DataType(DTE_NULL, "null");
DataType* DataType::DT_DOUBLE = new DataType(DTE_DOUBLE, "double", "0");
DataType* DataType::DT_STRING = new DataType(DTE_STRING, "string", "''");
DataType* DataType::DT_STRINGLIST = new DataType(DTE_STRINGLIST, "stringList", "<OBJECT>");
DataType* DataType::DT_NOT_DEFINED = new DataType(DTE_NOT_DEFINED, "<Not Defined>");

// ----------------------------------------------------------
// This constructor sets the default value of the new DataType
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType(DataTypeEnum type, string scopedTypeString, string defaultValue) 
	: type(type), defaultValue(defaultValue), scopedChildren(new DataTypeList()) { build(scopedTypeString); }

// ----------------------------------------------------------
// This constructor builds a well-defined DataType
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType(DataTypeEnum type, string scopedTypeString) 
	: type(type), defaultValue("<UNKNOWN>"), scopedChildren(new DataTypeList()) { build(scopedTypeString); }

// ----------------------------------------------------------
// This constructor builds an undefined DataType placeholder
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType() : type(DTE_NOT_DEFINED), scopeString(""), name(""), scopedChildren(new DataTypeList()) {}

// ----------------------------------------------------------
// This function builds the well-defined DataType tree
//
// Version 5.2
// ----------------------------------------------------------
void DataType::build(string scopedTypeString)
{
	size_t scopeOpLoc = scopedTypeString.find_last_of(SCOPE_OPERATOR);
	if (scopeOpLoc == string::npos)
	{
		scopeString = "";
		name = scopedTypeString;
		scopedParent = NULL;
	}
	else
	{
		scopeString = scopedTypeString.substr(0,scopeOpLoc);
		name = scopedTypeString.substr(scopeOpLoc + 1);
		scopedParent = new DataType(type, scopeString);
		scopedParent->scopedChildren->push_back(this);
	}
}

// ----------------------------------------------------------
// This function merges toAdd into the current DataType if
// applicable.
//
// Version 5.2
// ----------------------------------------------------------
void DataType::addRemainingTree(DataType * toAdd)
{
	if (toAdd == NULL)
		return;

	if (toAdd->scopeString == fullyScopedTypeString())
	{
		toAdd->scopedParent = this;
		scopedChildren->push_back(toAdd);
		return;
	}

	addRemainingTree(toAdd->scopedParent);
}

// ----------------------------------------------------------
// This function returns the fully scoped type string
//
// Version 5.2
// ----------------------------------------------------------
string DataType::fullyScopedTypeString() 
{ 
	if (scopeString == "") 
		return name;
	else
		return scopeString + SCOPE_OPERATOR + name;
}

// ----------------------------------------------------------
// This function adds the given well-defined type to the current
// DataType if applicable.
//
// Version 5.2
// ----------------------------------------------------------
void DataType::add(string scopedName)
{
	if (!matchesScope(scopedName))
		return;

	if (fullyScopedTypeString() == scopedName)
		return;

	int dtCount = scopedChildren->size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType * currDT = scopedChildren->at(dtIndex);
		if (currDT->matchesScope(scopedName))
		{
			currDT->add(scopedName);
			return;
		}
	}
	addRemainingTree(new DataType(DTE_USER_DEFINED, scopedName));
}

// ----------------------------------------------------------
// This function returns whether or not the given type is a 
// scoped decendent of the current DataType.
//
// Version 5.2
// ----------------------------------------------------------
bool DataType::isInTree(string scopedName)
{
	if (!matchesScope(scopedName))
		return false;

	if (fullyScopedTypeString() == scopedName)
		return true;

	int dtCount = scopedChildren->size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType * currDT = scopedChildren->at(dtIndex);
		if (currDT->matchesScope(scopedName))
			return currDT->isInTree(scopedName);
	}
	return false;
}

// ----------------------------------------------------------
// This function returns the corresponding DataType from the 
// current DataType's decendants. Returns DT_NOT_DEFINED if 
// the desired type is not a decendant.
//
// Version 5.2
// ----------------------------------------------------------
DataType * DataType::getDT(string scopedName)
{
	if (!matchesScope(scopedName))
		return DataType::DT_NOT_DEFINED;

	if (fullyScopedTypeString() == scopedName)
		return this;

	int dtCount = scopedChildren->size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType * currDT = scopedChildren->at(dtIndex);
		if (currDT->matchesScope(scopedName))
			return currDT->getDT(scopedName);
	}
	return DataType::DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function returns the upmost scoped parent.
//
// Version 5.2
// ----------------------------------------------------------
DataType * DataType::getRootDT()
{
	if (scopedParent == NULL)
		return this;

	return scopedParent->getRootDT();
}

// ----------------------------------------------------------
// This constructor populates the collection with built-in 
// DataTypes.
//
// Version 5.2
// ----------------------------------------------------------
DataTypeCollection::DataTypeCollection()
{
	//Add built in types
	push_back(DataType::DT_DOUBLE);
	push_back(DataType::DT_STRING);
	push_back(DataType::DT_STRINGLIST);
}

// ----------------------------------------------------------
// This function returns whether or not the given type is 
// contained within the collection.
//
// Version 5.2
// ----------------------------------------------------------
bool DataTypeCollection::isInList(string scopedName)
{
	int dtCount = size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType* currDT = at(dtIndex);
		if (currDT->matchesScope(scopedName))
			return currDT->isInTree(scopedName);
	}
	return false;
}

// ----------------------------------------------------------
// This function returns the corresponding DataType. Returns
// DT_NOT_DEFINED if the DataType is not in the collection.
//
// Version 5.2
// ----------------------------------------------------------
DataType * DataTypeCollection::getDT(string scopedName)
{
	int dtCount = size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType* currDT = at(dtIndex);
		if (currDT->matchesScope(scopedName))
			return currDT->getDT(scopedName);
	}
	return DataType::DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function adds the given type to collection.
//
// Version 5.2
// ----------------------------------------------------------
void DataTypeCollection::add(string scopedName)
{
	int dtCount = size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType * currDT = at(dtIndex);
		if (currDT->matchesScope(scopedName))
		{
			currDT->add(scopedName);
			return;
		}
	}
	
	push_back((new DataType(DTE_USER_DEFINED, scopedName))->getRootDT());
}