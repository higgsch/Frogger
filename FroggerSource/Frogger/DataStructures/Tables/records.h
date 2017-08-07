// -----------------------------------------------------------------
// This is the header for Record classes.
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include "..\dataTyped.h"
using namespace std;

// ----------------------------------------------------------
// This class represents everything known about a variable.
//
// Version 5.0
// ----------------------------------------------------------
struct Symbol
{
	string id;
	DataType * type;
	bool isLocal; //flag for a symbol defined within a user-defined function

	Symbol(string i_id, DataType * i_type, bool i_isLocal) 
		: id(i_id), type(i_type), isLocal(i_isLocal) {}

	bool equals(Symbol* s) { return id == s->id; }
	bool matches(Symbol* s) { return id == s->id; }
	//TODO fix matches
};

// ----------------------------------------------------------
// This class represents the data known about an argument.
//
// Version 5.0
// ----------------------------------------------------------
struct ArgPair
{
	string name;
	DataType * type;

	ArgPair(string argName, DataType * argType) : name(argName), type(argType) {}
};

// ----------------------------------------------------------
// This class represents the data known about an argument list.
//
// Version 4.2
// ----------------------------------------------------------
struct ArgList : vector<ArgPair *>
{
	bool isEmpty() { return size() == 0; }
};

// ----------------------------------------------------------
// This class represents everything known about a Routine.
//
// Version 5.0
// ----------------------------------------------------------
struct Routine
{
	bool builtIn;
	DataType * primary;
	string name;
	DataType * returnType;
	ArgList * args;

	Routine(DataType * primary, string name, DataType * returnType, bool builtIn)
		: primary(primary), name(name), returnType(returnType), builtIn(builtIn),
		  args(new ArgList()) {}

	void addArg(string argName, DataType* argType);
	
	bool equals(Routine* other);
	bool matches(Routine* other);
	void copy(Routine* other);

	int getNumArgs() { return args->size(); }
	ArgPair * getArg(int argNo);
	ArgPair * operator[](int index) { return (*args)[index]; }
	void setDataTypeOfArgNumber(int argNo, DataType * type);

	bool isUserDefined() { return !builtIn; }
};

//forward declarations
class ProgramNode;
class SymbolTable;

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function.
//
// Version 5.0
// ----------------------------------------------------------
struct UDFRecord : public Routine
{
	ProgramNode * root;
	SymbolTable * symbols;

	UDFRecord(DataType * primary, string name, DataType * returnType) : Routine(primary, name, returnType, false) {}
};

// ----------------------------------------------------------
// This class represents the data known about a User Defined 
// Function Collection.
//
// Version 4.2
// ----------------------------------------------------------
struct UDFCollection : vector<UDFRecord *> {};

//Forward declaration
struct ObjectStruct;

// ----------------------------------------------------------
// This class represents the data known about an Object File
// Collection.
//
// Version 5.0
// ----------------------------------------------------------
struct OFCollection : vector<ObjectStruct *> {};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Object. Generated from the .struct file.
//
// Version 5.0
// ----------------------------------------------------------
struct ObjectStruct
{
	string name;
	UDFCollection * UDFs;
	OFCollection * OFs;

	ObjectStruct() : UDFs(new UDFCollection()), OFs(new OFCollection()) {}

	int getNumberOfUDFs() { return UDFs->size(); }
	UDFRecord * getUDF(int udfIndex) { return (*UDFs)[udfIndex]; }
	ProgramNode* getUDFNode(int udfIndex) { return (*UDFs)[udfIndex]->root; }

	UDFRecord* operator[](int index) { return (*UDFs)[index]; }
};

// ----------------------------------------------------------
// This class represents the data known about a Frogger
// Program. Generated from the .struct file.
//
// Version 5.0
// ----------------------------------------------------------
struct ProgramStruct : public ObjectStruct
{
	UDFRecord * PEF;
	ProgramNode* getPEFNode() { return PEF->root; }
};