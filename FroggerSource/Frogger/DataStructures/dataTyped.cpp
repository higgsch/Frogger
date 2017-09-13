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
	: type(type), defaultValue(defaultValue), typeString(scopedTypeString), templatizers(NULL) {}

// ----------------------------------------------------------
// This constructor builds a well-defined DataType
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType(DataTypeEnum type, string scopedTypeString) 
	: type(type), defaultValue("<UNKNOWN>"), typeString(scopedTypeString), templatizers(NULL) {}

// ----------------------------------------------------------
// This constructor builds an undefined DataType placeholder
//
// Version 5.2
// ----------------------------------------------------------
DataType::DataType() : type(DTE_NOT_DEFINED), typeString(""), templatizers(NULL) {}

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
		if (currDT->typeString == scopedName)
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
		if (currDT->typeString == scopedName)
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
		if (currDT->typeString == scopedName)
			return;
	}
	
	push_back(new DataType(DTE_USER_DEFINED, scopedName));
}

bool DataType::operator==(const DataType& rhs)
{
	if (typeString != rhs.typeString)
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
	if (typeString != rhs.typeString)
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