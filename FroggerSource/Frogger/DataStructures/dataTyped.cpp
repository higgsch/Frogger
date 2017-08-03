#include "dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This function maps the DataType enum to strings
//
// Version 4.4
// ----------------------------------------------------------
string getDataTypeString(DataType dt)
{
	switch (dt)
	{
	case DT_NULL:
		return "null";
	case DT_DOUBLE:
		return "double";
	case DT_STRING:
		return "string";
	case DT_ARGS:
		return "args";
	default:
		return "<No Type>";
	}
}