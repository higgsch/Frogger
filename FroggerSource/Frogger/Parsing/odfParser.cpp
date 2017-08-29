//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the functionality to interpret a ODF file
// -----------------------------------------------------------------
#include "ODFParser.h"
using namespace std;

// ----------------------------------------------------------
// This function initiates the parse of the ODF and returns 
// the table of Data records.
// @dir: The OF/PF
// @name: The name of the object/project
//
// Version 5.0
// ----------------------------------------------------------
DataCollection * ODFParser::parseODF(string dir, string name)
{
	open(dir + name + ".data");

	DataCollection * dataStruct = new DataCollection();

	while (next_token().type != TT_SCANEOF)
	{
		DataRecord * currRec = dataRecord();

		if (isInData(currRec, dataStruct))
		{
			syntax_error("" + currRec->memberName + " data member is duplicated.");
		}
		else
		{
			dataStruct->push_back(currRec);
		}

		if (next_token().type != TT_SCANEOF)
			match(TT_EOL);
	}

	close();

	return dataStruct;
}

// ----------------------------------------------------------
// This function processes a data declaration record and 
// returns a single Data record.
//
// Version 5.0
// ----------------------------------------------------------
DataRecord * ODFParser::dataRecord()
{
	DataRecord * currRec = new DataRecord();

	currRec->memberName = id();
	match(TT_EQUAL_SIGN);
	currRec->type = dataType();

	Token tok = next_token();
	if (tok.type == TT_OCTOTHORPE)
	{
		currRec->defaultValue = defaultValue();
	}

	match(TT_SEMICOLON);

	return currRec;
}

// ----------------------------------------------------------
// This function processes the default value section of a 
// data declaration record.
//
// Version 5.0
// ----------------------------------------------------------
string ODFParser::defaultValue()
{
	string value = "";

	match(TT_OCTOTHORPE);

	Token tok = next_token();
	if (tok.type == TT_STRINGCONST || tok.type == TT_DOUBLECONST)
	{
		value = tok.lexeme;
		match(tok.type);
	}
	else if (tok.type == TT_OCTOTHORPE)
	{
		value = "<OBJECT>";
	}
	else 
	{
		syntax_error("Invalid default value: " + tok.lexeme);
	}
	
	match(TT_OCTOTHORPE);

	return value;
}

// ----------------------------------------------------------
// This function processes and returns a data type.
//
// Version 5.0
// ----------------------------------------------------------
DataType * ODFParser::dataType()
{
	Token type = next_token();
	match(TT_ID);

	if (type.lexeme == "double")
		return DataType::DT_DOUBLE;
	else if (type.lexeme == "string")
		return DataType::DT_STRING;
	else if (type.lexeme == "null")
		return DataType::DT_NULL;
	else if (type.lexeme == "stringList")
		return DataType::DT_STRINGLIST;
	else
	{
		return new DataType(DTE_USER_DEFINED, scope + type.lexeme);
	}
}

// ----------------------------------------------------------
// This function determines if the given DataRecord exists 
// in the data list.
// @rec: The DataRecord to compare to data.
// @data: A list of DataRecords
//
// Version 5.0
// ----------------------------------------------------------
bool ODFParser::isInData(DataRecord * rec, DataCollection * data)
{
	for (int recordIndex = 0; recordIndex < data->size(); recordIndex++)
	{
		DataRecord * currRec = data->at(recordIndex);

		if (rec->memberName == currRec->memberName)
			return true;
	}

	return false;
}