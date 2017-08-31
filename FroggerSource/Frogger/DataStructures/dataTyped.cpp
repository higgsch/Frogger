#include "dataTyped.h"
using namespace std;

DataType* DataType::DT_NULL = new DataType(DTE_NULL, "null");
DataType* DataType::DT_DOUBLE = new DataType(DTE_DOUBLE, "double", "0");
DataType* DataType::DT_STRING = new DataType(DTE_STRING, "string", "''");
DataType* DataType::DT_STRINGLIST = new DataType(DTE_STRINGLIST, "stringList", "<OBJECT>");
DataType* DataType::DT_NOT_DEFINED = new DataType(DTE_NOT_DEFINED, "<Not Defined>");