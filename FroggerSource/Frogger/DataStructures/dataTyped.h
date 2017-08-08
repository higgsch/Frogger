// -----------------------------------------------------------------
// This is the header file for the DataTyped class. 
// -----------------------------------------------------------------
#pragma once
#include <string>
#include <vector>
using namespace std;

//Set of node data types
enum DataTypeEnum
{
	DTE_NULL, //Used when the concept of a DataType is N/A 
	DTE_DOUBLE,DTE_STRING,DTE_STRINGLIST,
	DTE_USER_DEFINED,
	DTE_NOT_DEFINED //Used when the type is unknown
};

// ----------------------------------------------------------
// This class represents a data type.
//
// Version 5.0
// ----------------------------------------------------------
struct DataType
{
	DataTypeEnum type;
	string typeString; //The string representation of the datatype including full scope

	DataType(DataTypeEnum type, string typeString) : type(type), typeString(typeString) {}
	DataType() { type = DTE_NOT_DEFINED; typeString = ""; }

	bool operator==(const DataType& rhs) { return (type == rhs.type && typeString == typeString); }
	bool operator!=(const DataType& rhs) { return (typeString != rhs.typeString || type != rhs.type); }
	
	static DataType * DT_NULL;
	static DataType * DT_DOUBLE;
	static DataType * DT_STRING;
	static DataType * DT_STRINGLIST;
	static DataType * DT_NOT_DEFINED;

	bool isNull() { return type == DTE_NULL; }
	bool isDouble() { return type == DTE_DOUBLE; }
	bool isString() { return type == DTE_STRING; }
	bool isStringList() { return type == DTE_STRINGLIST; }
	bool isBuiltIn() { return isNull() || isDouble() || isString() || isStringList(); }
	bool isUserDefined() { return type == DTE_USER_DEFINED; }
	bool isDefined() { return type != DTE_NOT_DEFINED; }
};

// ----------------------------------------------------------
// This class mimics an interface, used for type checking.
//
// Version 5.0
// ----------------------------------------------------------
class DataTyped
{
protected:
	DataType * dataType; // the node's data type

public:
	DataTyped() { dataType = DataType::DT_NOT_DEFINED; }
	DataType * getDataType() { return dataType; }
	void setDataType(DataType * i_dataType) { dataType = i_dataType; }
	bool isTyped() { return dataType != DataType::DT_NOT_DEFINED; }
};

// ----------------------------------------------------------
// This class represents the collection of all available data 
// types
//
// Version 5.0
// ----------------------------------------------------------
struct DataTypeCollection : public vector<DataType*> 
{
	DataTypeCollection() 
	{
		//Add built in types
		push_back(DataType::DT_DOUBLE);
		push_back(DataType::DT_STRING);
		push_back(DataType::DT_STRINGLIST);
	}

	void add(string typeString)
	{
		if (!isInList(typeString))
			push_back(new DataType(DTE_USER_DEFINED, typeString));
	}

	bool isInList(string name)
	{
		int dtCount = size();
		for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
		{
			if (at(dtIndex)->typeString == name)
				return true;
		}
		return false;
	}

	DataType * getDT(string name)
	{
		int dtCount = size();
		for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
		{
			DataType* currDT = at(dtIndex);
			if (currDT->typeString == name)
				return currDT;
		}
		return DataType::DT_NOT_DEFINED;
	}
};