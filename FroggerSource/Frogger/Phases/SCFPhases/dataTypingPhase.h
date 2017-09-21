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
// Version 5.2
// ----------------------------------------------------------
class DataTypingPhase : SCFPhase
{
private:
	ProgramStruct * progStruct;

	DataTypeCollection * types;
	ObjectStruct * currObj;

	Language * lang;

	void populateAllTables(ObjectStruct* obj);
	bool typeExists(DataType * dt)
	{
		if (currObj->isTemplated())
		{
			int tCount = currObj->templateList->size();
			for (int tIndex = 0; tIndex < tCount; tIndex++)
			{
				
				string currTName = currObj->templateList->at(tIndex);
				if (currTName == dt->typeString())
					return true;
			}
		}

		if (dt->isTemplatized())
		{
			DataType * type = types->getDT(dt->scope + dt->name);
			return dt->templatizerList->size() == type->templatizerList->size();
		}
		else
		{
			DataType * type = types->getDT(dt->fullyScopedTypeString());
			return type->isDefined();
		}
	}

protected:
	void processODF(DataCollection * data)
	{
		int dataCount = data->size();
		for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
		{
			DataRecord* rec = data->at(dataIndex);
			if (!typeExists(rec->type))
				struct_error("ODF uses undefined type - " + rec->type->fullyScopedTypeString());
		}
	}

	void processArgs(ArgList* args)
	{
		int argCount = args->size();
		for (int argIndex = 0; argIndex < argCount; argIndex++)
		{
			ArgPair * arg = args->at(argIndex);
			if (!typeExists(arg->type))
				struct_error("UDF uses undefined argument type - " + arg->type->fullyScopedTypeString());
		}
	}

	void processUDF(UDFRecord * udf) 
	{
		if (!udf->returnType->isNull() && !typeExists(udf->returnType))
			struct_error("UDF has undefined return type - " + udf->returnType->fullyScopedTypeString());

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
			currObj = obj;
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