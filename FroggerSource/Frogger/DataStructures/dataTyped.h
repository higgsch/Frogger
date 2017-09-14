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

struct DataTypeCollection;

// ----------------------------------------------------------
// This class represents a data type.
//
// Version 5.2
// ----------------------------------------------------------
struct DataType
{
private:
	static const string SCOPE_OPERATOR;
	static const string OPEN_TEMPLATE_OPERATOR;
	static const string CLOSE_TEMPLATE_OPERATOR;

	void build(string fullTypeString);
	string extractScope(string fullTypeString);
	string extractName(string typeString);
	void buildTemplatizerList(string typeString);

	string templatizerString() const;

public:
	DataTypeEnum type;
	string scope; //The scope of the DataType
	string typeName; //The string representation of the datatype no scope
	string defaultValue; //The string representation of the default value
	DataTypeCollection * templatizerList;

	DataType(DataTypeEnum type, string fullTypeString, string defaultValue);
	DataType(DataTypeEnum type, string fullTypeString);
	DataType();

	string fullyScopedTypeString() const { return scope + typeName + templatizerString(); }
	string typeString() { return typeName + templatizerString(); }

	bool operator==(const DataType& rhs);
	bool operator!=(const DataType& rhs);
	
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
// Version 5.2
// ----------------------------------------------------------
struct DataTypeCollection : public vector<DataType*> 
{
	DataTypeCollection(bool addBuiltIn);

	void add(string scopedName);

	bool isInList(string scopedName);
	DataType * getDT(string scopedName);

	bool operator==(const DataTypeCollection& rhs);
	bool operator!=(const DataTypeCollection& rhs);
	
};