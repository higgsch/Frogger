//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.2
// -----------------------------------------------------------------
// This program provides for data typing
// -----------------------------------------------------------------
#include "dataTyped.h"
using namespace std;

const string DataType::SCOPE_OPERATOR = ":";
const string DataType::TEMPLATE_OPERATOR = "%";
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
DataType::DataType(DataTypeEnum type, string fullTypeString, string defaultValue) 
	: type(type), defaultValue(defaultValue) { build(fullTypeString); }

// ----------------------------------------------------------
// This constructor builds a well-defined DataType
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType(DataTypeEnum type, string fullTypeString) 
	: type(type), defaultValue("<UNKNOWN>"){ build(fullTypeString); }

// ----------------------------------------------------------
// This constructor builds an undefined DataType placeholder
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType() : type(DTE_NOT_DEFINED) { build(""); }

// ----------------------------------------------------------
// This function processes scope, typeString, and templatizers
// from the given full type string.
//
// Version 5.2
// ----------------------------------------------------------
void DataType::build(string fullTypeString)
{
	scope = extractScope(fullTypeString);
	typeString = fullTypeString.substr(scope.length());
	templatizers = NULL;
}

// ----------------------------------------------------------
// This function returns the scope from the given full type string
// including final scope operator, empty string if scope DNE.
//
// Version 5.2
// ----------------------------------------------------------
string DataType::extractScope(string fullTypeString)
{
	//Strip off templatization - no change if not templatized
	size_t lastTemplateOp = fullTypeString.find_last_of(TEMPLATE_OPERATOR);
	size_t secondToLastTemplateOp = fullTypeString.substr(0, lastTemplateOp).find_last_of(TEMPLATE_OPERATOR);
	fullTypeString = fullTypeString.substr(0, secondToLastTemplateOp);
	
	size_t lastScopeOp = fullTypeString.find_last_of(SCOPE_OPERATOR);
	if (lastScopeOp == string::npos)
		return "";
	else
		return fullTypeString.substr(0, lastScopeOp + 1);
}

// ----------------------------------------------------------
// This constructor populates the collection with built-in 
// DataTypes.
//
// Version 5.2
// ----------------------------------------------------------
DataTypeCollection::DataTypeCollection(bool addBuiltIn)
{
	if (addBuiltIn)
	{
		push_back(DataType::DT_DOUBLE);
		push_back(DataType::DT_STRING);
		push_back(DataType::DT_STRINGLIST);
	}
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
		if (currDT->fullyScopedTypeString() == scopedName)
			return true;
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
		if (currDT->fullyScopedTypeString() == scopedName)
			return currDT;
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
		if (currDT->fullyScopedTypeString() == scopedName)
			return;
	}
	
	push_back(new DataType(DTE_USER_DEFINED, scopedName));
}

bool DataType::operator==(const DataType& rhs)
{
	if (fullyScopedTypeString() != rhs.fullyScopedTypeString())
		return false;

	if (templatizers == NULL)
	{
		if (rhs.templatizers == NULL || rhs.templatizers->size() == 0)
			return true;
		else
			return false;
	}
	else if (rhs.templatizers == NULL)
	{
		if (templatizers->size() == 0)
			return true;
		else
			return false;
	}
	
	return *(templatizers) == *(rhs.templatizers);
}

bool DataType::operator!=(const DataType& rhs)
{
	if (fullyScopedTypeString() != rhs.fullyScopedTypeString())
		return true;

	if (templatizers == NULL)
	{
		if (rhs.templatizers == NULL || rhs.templatizers->size() == 0)
			return false;
		else 
			return true;
	}
	else if (rhs.templatizers == NULL)
	{
		if (templatizers->size() == 0)
			return false;
		else
			return true;
	}
	
	return *(templatizers) != *(rhs.templatizers);
}

bool DataTypeCollection::operator==(const DataTypeCollection& rhs)
{
	if (size() != rhs.size())
		return false;

	int dtCount = size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		if (*(at(dtIndex)) != *(rhs.at(dtIndex)))
			return false;
	}
	return true;
}

bool DataTypeCollection::operator!=(const DataTypeCollection& rhs)
{
	if (size() != rhs.size())
		return true;

	int dtCount = size();
	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		if (*(at(dtIndex)) != *(rhs.at(dtIndex)))
			return true;
	}
	return false;
}