// -----------------------------------------------------------------
// This is the header file for the DataTyped class. 
// -----------------------------------------------------------------
#pragma once

using namespace std;

//Set of node data types
enum DataType
{
	DT_NULL, //Used when the concept of a DataType is N/A 
	DT_DOUBLE,DT_STRING,
	DT_NOT_DEFINED //Used when the type is unknown
};

// ----------------------------------------------------------
// This class mimics an interface, used for type checking.
//
// Version 3.0
// ----------------------------------------------------------
class DataTyped
{
protected:
	DataType dataType; // the node's data type

public:
	DataTyped() { dataType = DT_NOT_DEFINED; }
	DataType getDataType() { return dataType; }
	void setDataType(DataType i_dataType) { dataType = i_dataType; }
	bool isTyped() { return dataType != DT_NOT_DEFINED; }
};