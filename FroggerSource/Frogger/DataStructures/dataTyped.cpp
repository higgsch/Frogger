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
// @fullTypeString: The complete, scoped type string
//
// Version 5.2
// ----------------------------------------------------------
void DataType::build(string fullTypeString)
{
	scope = extractScope(fullTypeString);
	string typeString = fullTypeString.substr(scope.length());
	typeName = extractName(typeString);
	buildTemplatizerList(typeString);
}

// ----------------------------------------------------------
// This function processes templatizers from the given full 
// type string.
// @typeString: The complete type string without scope
//
// Version 5.2
// ----------------------------------------------------------
void DataType::buildTemplatizerList(string typeString)
{
	templatizerList = new DataTypeCollection(false);

	size_t firstTemplateOp = typeString.find(TEMPLATE_OPERATOR);
	size_t lastTemplateOp = typeString.find_last_of(TEMPLATE_OPERATOR);
	if (firstTemplateOp == string::npos)
		return;

	string templateString = typeString.substr(firstTemplateOp + 1, lastTemplateOp - firstTemplateOp - 1);
	size_t comma = templateString.find(",");
	while (comma != string::npos)
	{
		templatizerList->add(templateString.substr(0,comma));

		templateString = templateString.substr(comma + 1);
		comma = templateString.find(",");
	}

	templatizerList->add(templateString);
}

// ----------------------------------------------------------
// This function returns the scope from the given full type string
// including final scope operator, empty string if scope DNE.
// @fullTypeString: The complete, scoped type string
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
// This function returns the type name from the given type string
// without templatizer string.
// @typeString: The type string without scope
//
// Version 5.2
// ----------------------------------------------------------
string DataType::extractName(string typeString)
{
	size_t firstTemplatizerOp = typeString.find(TEMPLATE_OPERATOR);
	return typeString.substr(0, firstTemplatizerOp);
}

// ----------------------------------------------------------
// This function returns the string version of the templatizer
// list.
//
// Version 5.2
// ----------------------------------------------------------
string DataType::templatizerString() const
{
	if (templatizerList->size() == 0)
		return "";

	string result = TEMPLATE_OPERATOR + templatizerList->at(0)->fullyScopedTypeString();

	int tCount = templatizerList->size();
	for (int tIndex = 1; tIndex < tCount; tIndex++)
	{
		result += ", " + templatizerList->at(tIndex)->fullyScopedTypeString();
	}
	
	return result + TEMPLATE_OPERATOR;
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

	for (int dtIndex = 0; dtIndex < dtCount; dtIndex++)
	{
		DataType* currDT = at(dtIndex);
		if (currDT->scope + currDT->typeName == scopedName)
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
	
	return *(templatizerList) == *(rhs.templatizerList);
}

bool DataType::operator!=(const DataType& rhs)
{
	if (fullyScopedTypeString() != rhs.fullyScopedTypeString())
		return true;
	
	return *(templatizerList) != *(rhs.templatizerList);
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