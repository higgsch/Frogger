// -----------------------------------------------------------------
// This is the header for the DataTypingPhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include "..\FGRPhases\fgrDataTypingPhase.h"
#include "..\..\Languages\language.h"
using namespace std;

// ----------------------------------------------------------
// This class ensures data typing rules are upheld.
//
// Version 5.1
// ----------------------------------------------------------
class DataTypingPhase : SCFPhase
{
private:
	ProgramStruct * progStruct;

	DataTypeCollection * types;

	Language * lang;

	void populateAllTables(ObjectStruct* obj);
	bool typeExists(DataType * dt)
	{
		DataType * type = types->getDT(dt->typeString);
		return type->isDefined();
	}

protected:
	void processODF(DataCollection * data)
	{
		int dataCount = data->size();
		for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
		{
			DataRecord* rec = data->at(dataIndex);
			if (!typeExists(rec->type))
				struct_error("ODF uses undefined type - " + rec->type->typeString);
		}
	}

	void processArgs(ArgList* args)
	{
		int argCount = args->size();
		for (int argIndex = 0; argIndex < argCount; argIndex++)
		{
			ArgPair * arg = args->at(argIndex);
			if (!typeExists(arg->type))
				struct_error("UDF uses undefined argument type - " + arg->type->typeString);
		}
	}

	void processUDF(UDFRecord * udf) 
	{
		if (udf->returnType->typeString != "null" && !typeExists(udf->returnType))
			struct_error("UDF has undefined return type - " + udf->returnType->typeString);

		processArgs(udf->args);

		SymbolTable * symbols = new SymbolTable(lang, udf);
		udf->visibleTables->syms->merge(symbols);
		FGRDataTypingPhase dtp(this, lang, udf->visibleTables, udf->name, udf->primary);
		udf->root->accept(&dtp);
	}

	void processOF(ObjectStruct * obj) 
	{
		if (obj->isUserDefined)
		{
			processODF(obj->data);
			runForLocalUDFs(obj);
			runForLocalOFs(obj);
		}
	}

public:
	TableGroup * allTables;

	void process(ProgramStruct * prog)
	{
		progStruct = prog;
		types = progStruct->types;

		populateAllTables(prog);

		processUDF(progStruct->PEF);
		processOF(prog);
	}

	TableGroup * getScopedTables(DataType * primary)
	{
		return progStruct->getObject(primary)->scopedTables;
	}

	TableGroup * getParentScopedTables(DataType * primary)
	{
		DataType * parentType = getParentType(primary);
		return getScopedTables(parentType);
	}

	DataType * getParentType(DataType * type)
	{
		ObjectStruct * obj = progStruct->getObject(type);
		ObjectStruct * parentObj = obj->parent;
		return parentObj->getUDF(0)->primary;
	}

	DataTypingPhase(Language * lang) : lang(lang), allTables(new TableGroup()) {}
};