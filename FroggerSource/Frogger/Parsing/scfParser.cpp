//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
// -----------------------------------------------------------------
// This program provides the functionality to interpret a SCF file
// -----------------------------------------------------------------
#include "SCFParser.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 4.0
// ----------------------------------------------------------
SCFParser::SCFParser()
{
	current_token = SCFToken::NOTOK;
	lookahead[0] = SCFToken::NOTOK;
}

// ----------------------------------------------------------
// This function initiates the parse of the SCF and returns 
// the table of UDF records.
//
// Version 4.0
// ----------------------------------------------------------
vector<UDFRecord *> * SCFParser::parseSCF(string SCFPath, string projectName)
{
	open(SCFPath);

	vector<UDFRecord *> * files = new vector<UDFRecord *>();

	while (next_token().type != TOKTYPE_SCANEOF)
	{
		UDFRecord * currRec = record();
		if (!isPEF(currRec, projectName))
		{
			files->push_back(currRec);
		}
		if (next_token().type != TOKTYPE_SCANEOF)
			match(TOKTYPE_EOL);
	}

	close();

	return files;
}

// ----------------------------------------------------------
// This function processes and returns a single UDF record.
//
// Version 4.0
// ----------------------------------------------------------
UDFRecord * SCFParser::record()
{
	UDFRecord * rec = new UDFRecord();

	rec->UDFName = filename();
	rec->args = arguments();
	match(TOKTYPE_ARROW);
	rec->returnType = dataType();

	return rec;
}

// ----------------------------------------------------------
// This function processes and returns a list of arguments.
//
// Version 4.0
// ----------------------------------------------------------
vector<argPair *> * SCFParser::arguments()
{
	vector<argPair *> * args = new vector<argPair *>();

	if (next_token().type != TOKTYPE_ARROW)
	{
		args->push_back(argument());
	}

	while (next_token().type == TOKTYPE_COMMA)
	{
		match(TOKTYPE_COMMA);
		args->push_back(argument());
	}

	return args;
}

// ----------------------------------------------------------
// This function processes and returns a single argument.
//
// Version 4.0
// ----------------------------------------------------------
argPair * SCFParser::argument()
{
	argPair * arg = new argPair();

	SCFToken name = next_token();
	match(TOKTYPE_ID);
	arg->name = name.lexeme;

	match(TOKTYPE_COLON);
	arg->type = dataType();

	return arg;
}

// ----------------------------------------------------------
// This function processes and returns a filename.
//
// Version 4.0
// ----------------------------------------------------------
string SCFParser::filename()
{
	SCFToken name = next_token();
	match(TOKTYPE_ID);
	SCFToken dot = next_token();
	match(TOKTYPE_DOT);
	SCFToken ext = next_token();
	match(TOKTYPE_ID);

	if (ext.lexeme != "fgr")
		syntax_error("Found ." + ext.lexeme + " -- Expected .fgr");

	//return name.lexeme + dot.lexeme + ext.lexeme;
	return name.lexeme;
}

// ----------------------------------------------------------
// This function processes and returns a data type.
//
// Version 4.0
// ----------------------------------------------------------
DataType SCFParser::dataType()
{
	SCFToken type = next_token();
	match(TOKTYPE_ID);

	if (type.lexeme == "double")
		return DT_DOUBLE;
	else if (type.lexeme == "string")
		return DT_STRING;
	else if (type.lexeme == "null")
		return DT_NULL;
	else
		syntax_error("Invalid data type -- Expected double, string, or null");

	return DT_NOT_DEFINED;
}

// ----------------------------------------------------------
// This function determines if the given UDFRecord matches 
// the required PEF.
// @rec: The UDFRecord to compare to PEF.
//
// Version 4.0
// ----------------------------------------------------------
bool SCFParser::isPEF(UDFRecord * rec, string pefName)
{
	if (rec->returnType != DT_NULL)
		return false;

	if (rec->UDFName != pefName)
		return false;

	if (rec->args != NULL && rec->args->size() != 0)
		return false;

	return true;
}

// ----------------------------------------------------------
// This function tests if the next token matches toMatch and
// moves to the next token on success. It displayes an error
// on failure.
// @toMatch: The expected token category.
//
// Version 4.0
// ----------------------------------------------------------
void SCFParser::match(scf_token_type toMatch)
{
	SCFToken tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = SCFToken::NOTOK;
	}
	else
	{
		string type;
		switch (toMatch)
		{
		case TOKTYPE_DOT:
			type = ".";
			break;
		case TOKTYPE_COLON:
			type = ":";
			break;
		case TOKTYPE_COMMA:
			type = ",";
			break;
		case TOKTYPE_ARROW:
			type = "->";
			break;
		case TOKTYPE_SCANEOF:
			type = "<EOF>";
			break;
		case TOKTYPE_ID:
			type = "Identifier";
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
// Version 4.0
// ----------------------------------------------------------
void SCFParser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "STRUCT SYNTAX ERROR on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 4.0
// ----------------------------------------------------------
SCFToken SCFParser::next_token()
{
	if (lookahead[0].type == TOKTYPE_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}