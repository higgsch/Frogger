//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the functionality to interpret a ODF file
// -----------------------------------------------------------------
#include "ODFParser.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 5.0
// ----------------------------------------------------------
ODFParser::ODFParser()
{
	current_token = ODFToken::NOTOK;
	lookahead[0] = ODFToken::NOTOK;
	currFilePath = "";
}

// ----------------------------------------------------------
// This function initiates the parse of the ODF and returns 
// the table of Data records.
// @dir: The OF/PF
// @name: The name of the object/project
//
// Version 5.0
// ----------------------------------------------------------
DataStruct * ODFParser::parseODF(string dir, string name)
{
	open(dir + name + ".data");

	DataStruct * dataStruct = new DataStruct();

	while (next_token().type != ODFTT_SCANEOF)
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

		if (next_token().type != ODFTT_SCANEOF)
			match(ODFTT_EOL);
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
	match(ODFTT_EQUALS);
	currRec->type = dataType();

	ODFToken tok = next_token();
	if (tok.type == ODFTT_OCTOTHORPE)
	{
		currRec->defaultValue = defaultValue();
	}

	match(ODFTT_SEMICOLON);

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

	match(ODFTT_OCTOTHORPE);

	ODFToken tok = next_token();
	if (tok.type == ODFTT_STRING || tok.type == ODFTT_DOUBLECONST)
	{
		value = tok.lexeme;
		match(tok.type);
	}
	else if (tok.type == ODFTT_OCTOTHORPE)
	{
		value = "<OBJECT>";
	}
	else 
	{
		syntax_error("Invalid default value: " + tok.lexeme);
	}
	
	match(ODFTT_OCTOTHORPE);

	return value;
}

// ----------------------------------------------------------
// This function processes and returns a function name.
//
// Version 5.0
// ----------------------------------------------------------
string ODFParser::id()
{
	ODFToken id = next_token();
	match(ODFTT_ID);

	return id.lexeme;
}

// ----------------------------------------------------------
// This function processes and returns a data type.
//
// Version 5.0
// ----------------------------------------------------------
DataType * ODFParser::dataType()
{
	ODFToken type = next_token();
	match(ODFTT_ID);

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
		return new DataType(DTE_USER_DEFINED, type.lexeme);
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
bool ODFParser::isInData(DataRecord * rec, DataStruct * data)
{
	for (int recordIndex = 0; recordIndex < data->size(); recordIndex++)
	{
		DataRecord * currRec = data->at(recordIndex);

		if (rec->memberName == currRec->memberName)
			return true;
	}

	return false;
}

// ----------------------------------------------------------
// This function tests if the next token matches toMatch and
// moves to the next token on success. It displayes an error
// on failure.
// @toMatch: The expected token category.
//
// Version 5.0
// ----------------------------------------------------------
void ODFParser::match(odf_token_type toMatch)
{
	ODFToken tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = ODFToken::NOTOK;
	}
	else
	{
		string type;
		switch (toMatch)
		{
		case ODFTT_EQUALS:
			type = ODFToken::EQUALS.lexeme;
			break;
		case ODFTT_OCTOTHORPE:
			type = ODFToken::OCTOTHORPE.lexeme;
			break;
		case ODFTT_SEMICOLON:
			type = ODFToken::SEMICOLON.lexeme;
			break;
		case ODFTT_SCANEOF:
			type = "<EOF>";
			break;
		case ODFTT_ID:
			type = "Identifier";
			break;
		case ODFTT_EOL:
			type = "End of Line";
			break;
		default:
			type = "<type>";
			break;
		}

		syntax_error("Expected \'" + type + "\' - Found " + lookahead[0].lexeme);
	}
}

// ----------------------------------------------------------
// This function displays an error to the user and terminates
// the program.
// @err_msg: The message to display.
//
// Version 5.0
// ----------------------------------------------------------
void ODFParser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "STRUCT SYNTAX ERROR in file " << currFilePath << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 5.0
// ----------------------------------------------------------
ODFToken ODFParser::next_token()
{
	if (lookahead[0].type == ODFTT_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}

// ----------------------------------------------------------
// This function opens the ODF and checks for existance.
// @ODFPath: The path to the ODF.
//
// Version 5.0
// ----------------------------------------------------------
void ODFParser::open(string ODFPath)
{
	scanner.open(ODFPath);

	if (!scanner.good())
	{
		scanner.close();
		syntax_error("ODF not found: " + ODFPath);
	}

	currFilePath = ODFPath;
}