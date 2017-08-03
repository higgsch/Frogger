//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.4
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
// Version 4.2
// ----------------------------------------------------------
UDFCollection * SCFParser::parseSCF(string SCFPath, string projectName)
{
	open(SCFPath);

	UDFCollection * files = new UDFCollection();

	while (next_token().type != TOKTYPE_SCANEOF)
	{
		UDFRecord * currRec = record();
		if (!isPEF(currRec, projectName))
		{
			if (isInFiles(currRec, files))
			{
				syntax_error("" + currRec->UDFName + " function is duplicated.");
			}
			else
			{
				files->push_back(currRec);
			}
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
// Version 4.4
// ----------------------------------------------------------
UDFRecord * SCFParser::record()
{
	UDFRecord * rec = new UDFRecord();

	rec->UDFName = functName();
	match(TOKTYPE_LPAREN);
	rec->args = arguments();
	match(TOKTYPE_RPAREN);
	match(TOKTYPE_TILDE);
	rec->returnType = dataType();

	match(TOKTYPE_DOT);
	SCFToken ext = next_token();
	match(TOKTYPE_ID);

	if (ext.lexeme != "fgr")
		syntax_error("Found ." + ext.lexeme + " -- Expected .fgr");

	return rec;
}

// ----------------------------------------------------------
// This function processes and returns a function name.
//
// Version 4.4
// ----------------------------------------------------------
string SCFParser::functName()
{
	SCFToken name = next_token();
	match(TOKTYPE_ID);

	return name.lexeme;
}

// ----------------------------------------------------------
// This function processes and returns a list of arguments.
//
// Version 4.4
// ----------------------------------------------------------
ArgList * SCFParser::arguments()
{
	ArgList * args = new ArgList();

	if (next_token().type != TOKTYPE_RPAREN)
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
// Version 4.4
// ----------------------------------------------------------
ArgPair * SCFParser::argument()
{
	string name = next_token().lexeme;
	match(TOKTYPE_ID);
	match(TOKTYPE_EQUALS);
	DataType dt = dataType();

	return new ArgPair(name, dt);
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
// Version 4.2
// ----------------------------------------------------------
bool SCFParser::isPEF(UDFRecord * rec, string pefName)
{
	if (rec->returnType != DT_NULL)
		return false;

	if (rec->UDFName != pefName)
		return false;

	if (rec->args->size() != 0)
		return false;

	return true;
}

// ----------------------------------------------------------
// This function determines if the given UDFRecord exists 
// in the file list.
// @rec: The UDFRecord to compare to files.
// @files: A list of files
//
// Version 4.4
// ----------------------------------------------------------
bool SCFParser::isInFiles(UDFRecord * rec, UDFCollection * files)
{
	for (int recordIndex = 0; recordIndex < files->size(); recordIndex++)
	{
		UDFRecord * currRec = files->at(recordIndex);

		if (functionSignatureMatches(currRec, rec))
			return true;
	}

	return false;
}

// ----------------------------------------------------------
// This function determines if the given UDFRecords' signatures
// match. 
// @first: The UDFRecord to compare to second.
// @second: The UDFRecord to compare to first.
//
// Version 4.4
// ----------------------------------------------------------
bool SCFParser::functionSignatureMatches(UDFRecord * first, UDFRecord * second)
{
	if (first->UDFName != second->UDFName)
		return false;

	int argCount = first->args->size();
	if (argCount != second->args->size())
		return false;

	for (int argIndex = 0; argIndex < argCount; argIndex++)
	{
		if (first->args->at(argIndex)->type != second->args->at(argIndex)->type)
			return false;
	}

	return true;
}

// ----------------------------------------------------------
// This function tests if the next token matches toMatch and
// moves to the next token on success. It displayes an error
// on failure.
// @toMatch: The expected token category.
//
// Version 4.4
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
		case TOKTYPE_LPAREN:
			type = SCFToken::LPAREN.lexeme;
			break;
		case TOKTYPE_RPAREN:
			type = SCFToken::RPAREN.lexeme;
			break;
		case TOKTYPE_EQUALS:
			type = SCFToken::EQUALS.lexeme;
			break;
		case TOKTYPE_COMMA:
			type = SCFToken::COMMA.lexeme;
			break;
		case TOKTYPE_TILDE:
			type = SCFToken::TILDE.lexeme;
			break;
		case TOKTYPE_DOT:
			type = SCFToken::DOT.lexeme;
			break;
		case TOKTYPE_SCANEOF:
			type = "<EOF>";
			break;
		case TOKTYPE_ID:
			type = "Identifier";
			break;
		case TOKTYPE_EOL:
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
// Version 4.4
// ----------------------------------------------------------
void SCFParser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "STRUCT SYNTAX ERROR on line " << lineNo << ": " << err_msg << endl;
	
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
// Version 4.0
// ----------------------------------------------------------
SCFToken SCFParser::next_token()
{
	if (lookahead[0].type == TOKTYPE_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}