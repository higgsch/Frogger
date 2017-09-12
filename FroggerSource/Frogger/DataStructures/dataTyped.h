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

struct DataType;
struct DataTypeList : vector<DataType *> {};

// ----------------------------------------------------------
// This class represents a data type.
//
// Version 5.2
// ----------------------------------------------------------
struct DataType
{
private:
	static const string SCOPE_OPERATOR;

public:
	DataType * scopedParent;
	DataTypeList * scopedChildren;

	DataTypeEnum type;
	string scopeString; //The string representation full scope
	string name; //The string representation of the datatype
	string defaultValue; //The string representation of the default value

	DataType(DataTypeEnum type, string scopedTypeString, string defaultValue);
	DataType(DataTypeEnum type, string scopedTypeString);
	DataType();

	string fullyScopedTypeString();

	bool matchesScope(string scopedName) { return scopedName.find(fullyScopedTypeString()) == 0; }

	bool operator==(const DataType& rhs) { return scopeString == rhs.scopeString && name == rhs.name; }
	bool operator!=(const DataType& rhs) { return name != rhs.name || scopeString != rhs.scopeString; }
	
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

	void add(string scopedName);
	bool isInTree(string scopedName);
	DataType * getDT(string scopedName);
	DataType * getRootDT();

private:
	void build(string scopedTypeString);
	void addRemainingTree(DataType * toAdd);
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
// Version 5.2
// ----------------------------------------------------------
struct DataTypeCollection : public vector<DataType*> 
{
	DataTypeCollection();

	void add(string scopedName);

	bool isInList(string scopedName);
	DataType * getDT(string scopedName);
};