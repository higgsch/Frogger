//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides the functionality to interpret a SCF file
// -----------------------------------------------------------------
#include "SCFParser.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 5.0
// ----------------------------------------------------------
SCFParser::SCFParser(Language * lang) : lang(lang)
{
	current_token = SCFToken::NOTOK;
	lookahead[0] = SCFToken::NOTOK;
	currFilePath = "";

	types = new DataTypeCollection();
}

// ----------------------------------------------------------
// This function initiates the parse of the SCF and returns 
// the table of UDF records.
// @projectDir: The PF
// @projectName: The name of the project
//
// Version 5.0
// ----------------------------------------------------------
ProgramStruct * SCFParser::parseProgramLevelSCF(string projectDir, string projectName)
{
	open(projectDir + projectName + ".struct");

	ProgramStruct * progStruct = new ProgramStruct(lang);
	progStruct->name = projectName;
	progStruct->PEF = NULL;

	while (next_token().type != SCFTT_SCANEOF)
	{
		string name = id();

		if (next_token().type == SCFTT_LPAREN)
		{ //Function Declaration Record
			UDFRecord * currRec = functRecord(name);
			if (isPEF(currRec, projectName))
			{
				currRec->visibleCmds->addEndNull();

				if (progStruct->PEF != NULL)
					syntax_error("PEF is duplicated.");

				progStruct->PEF = currRec;
				progStruct->scopedCmds->add(currRec);
			}
			else
			{
				if (isInFunctions(currRec, progStruct->UDFs))
				{
					syntax_error("" + currRec->name + " function is duplicated.");
				}
				
				progStruct->UDFs->push_back(currRec);

				addEndCommand(currRec);
				if (currRec->returnType->isNull())
				{
					progStruct->scopedCmds->add(currRec);
				}
				else
				{
					progStruct->scopedFuncts->add(currRec);
				}
			}
		}
		else if (next_token().type == SCFTT_DOT)
		{ //Object Declaration Record or Data Declaration Record
			match(SCFTT_DOT);

			SCFToken ext = next_token();
			match(SCFTT_ID);

			if (ext.lexeme == "struct")
			{
				ObjectStruct * currRec = objectRecord(projectDir + name + "\\", name);
			
				if (isInObjects(currRec, progStruct->OFs))
				{
					syntax_error(currRec->name + " object is duplicated.");
				}
				
				progStruct->OFs->push_back(currRec);

			}
			else if (ext.lexeme == "data")
			{
				if (name != projectName)
				{
					syntax_error("Invalid ODF: " + name + ".data -- Expected " + projectName + ".data");
				}
				
				progStruct->data = dataRecord(projectDir, projectName);

				int dataCount = progStruct->data->size();
				for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
				{
					DataRecord * currData = progStruct->data->at(dataIndex);
					progStruct->scopedSymbols->add(new SymbolRecord(currData->memberName, currData->type, false));
				}
			}
			else
				syntax_error("Found " + name + "." + ext.lexeme + " -- Expected " + name + ".struct or " + projectName + ".data");
		}
		else
		{
			syntax_error("Incomplete SCF Record: " + name);
		}

		if (next_token().type != SCFTT_SCANEOF)
			match(SCFTT_EOL);
	}

	close();

	return progStruct;
}

// ----------------------------------------------------------
// This function initiates the parse of the SCF and returns 
// the ObjectStructure
// @objectDir: The OF
// @objectName: The name of the object
//
// Version 5.0
// ----------------------------------------------------------
ObjectStruct * SCFParser::parseObjectLevelSCF(string objectDir, string objectName)
{
	types->add(objectName);
	open(objectDir + objectName + ".struct");

	ObjectStruct * objStruct = new ObjectStruct(lang);
	objStruct->name = objectName;

	//ODFParser dataP;
	//objStruct->data = dataP.parseODF(objectDir, objectName);

	while (next_token().type != SCFTT_SCANEOF)
	{
		string name = id();

		if (next_token().type == SCFTT_LPAREN)
		{ //Function Declaration Record
			UDFRecord * currRec = functRecord(name);
			currRec->primary = types->getDT(objectName);
			if (isInFunctions(currRec, objStruct->UDFs))
			{
				syntax_error("" + currRec->name + " function is duplicated.");
			}
			
			objStruct->UDFs->push_back(currRec);

			addEndCommand(currRec);
			if (currRec->returnType->isNull())
			{
				objStruct->scopedCmds->add(currRec);
			}
			else
			{
				objStruct->scopedFuncts->add(currRec);
			}
		}
		else if (next_token().type == SCFTT_DOT)
		{ //Object Declaration Record or Data Declaration Record
			match(SCFTT_DOT);

			SCFToken ext = next_token();
			match(SCFTT_ID);

			if (ext.lexeme == "struct")
			{
				ObjectStruct * currRec = objectRecord(objectDir + name + "\\", name);
			
				if (isInObjects(currRec, objStruct->OFs))
				{
					syntax_error(currRec->name + " object is duplicated.");
				}
				
				objStruct->OFs->push_back(currRec);
			}
			else if (ext.lexeme == "data")
			{
				if (name != objectName)
				{
					syntax_error("Invalid ODF: " + name + ".data -- Expected " + objectName + ".data");
				}
				
				objStruct->data = dataRecord(objectDir, objectName);

				int dataCount = objStruct->data->size();
				for (int dataIndex = 0; dataIndex < dataCount; dataIndex++)
				{
					DataRecord * currData = objStruct->data->at(dataIndex);
					objStruct->scopedSymbols->add(new SymbolRecord(currData->memberName, currData->type, false));
				}
			}
			else
				syntax_error("Found " + name + "." + ext.lexeme + " -- Expected " + name + ".struct or " + objectName + ".data");
		}
		else
		{
			syntax_error("Incomplete SCF Record: " + name);
		}

		if (next_token().type != SCFTT_SCANEOF)
			match(SCFTT_EOL);
	}

	close();

	return objStruct;
}

// ----------------------------------------------------------
// This function adds the correct end command to the given 
// UDFRecord.
// @rec: The record to add an end command to.
//
// Version 5.0
// ----------------------------------------------------------
void SCFParser::addEndCommand(UDFRecord* rec)
{
	if (rec->returnType->isNull())
		rec->visibleCmds->addEndNull();
	else if (rec->returnType->isDouble())
		rec->visibleCmds->addEndDouble();
	else if (rec->returnType->isString())
		rec->visibleCmds->addEndString();
	else
	{
		Routine* endCommand = new Routine(rec->primary, "end", DataType::DT_NULL, true);
		endCommand->addArg("", rec->returnType);

		rec->visibleCmds->add(endCommand);
	}
}

// ----------------------------------------------------------
// This function processes an object declaration record and 
// returns a single Object Structure.
// @objectDir: The OF including trailing slash
// @name: The objectName
//
// Version 5.0
// ----------------------------------------------------------
ObjectStruct * SCFParser::objectRecord(string objectDir, string name)
{
	SCFParser p(lang);
	return p.parseObjectLevelSCF(objectDir, name);
}

// ----------------------------------------------------------
// This function processes a data declaration record and 
// returns a single Data Structure.
// @dataDir: The OF/PF including trailing slash
// @name: The objectName/projectName
//
// Version 5.0
// ----------------------------------------------------------
DataStruct * SCFParser::dataRecord(string dataDir, string name)
{
	ODFParser p;
	return p.parseODF(dataDir, name);
}

// ----------------------------------------------------------
// This function processes a function declaration record and 
// returns a single UDF record.
// @name: The UDFName
//
// Version 5.0
// ----------------------------------------------------------
UDFRecord * SCFParser::functRecord(string name)
{
	match(SCFTT_LPAREN);
	ArgList * args = arguments();
	match(SCFTT_RPAREN);
	match(SCFTT_TILDE);
	DataType * returnType = dataType();

	match(SCFTT_DOT);
	SCFToken ext = next_token();
	match(SCFTT_ID);

	if (ext.lexeme != "fgr")
		syntax_error("Found ." + ext.lexeme + " -- Expected .fgr");

	UDFRecord * rec = new UDFRecord(DataType::DT_NULL, name, returnType, lang);
	rec->args = args;
	return rec;
}

// ----------------------------------------------------------
// This function processes and returns a function name.
//
// Version 5.0
// ----------------------------------------------------------
string SCFParser::id()
{
	SCFToken id = next_token();
	match(SCFTT_ID);

	return id.lexeme;
}

// ----------------------------------------------------------
// This function processes and returns a list of arguments.
//
// Version 5.0
// ----------------------------------------------------------
ArgList * SCFParser::arguments()
{
	ArgList * args = new ArgList();

	if (next_token().type != SCFTT_RPAREN)
	{
		args->push_back(argument());
	}

	while (next_token().type == SCFTT_COMMA)
	{
		match(SCFTT_COMMA);
		args->push_back(argument());
	}

	return args;
}

// ----------------------------------------------------------
// This function processes and returns a single argument.
//
// Version 5.0
// ----------------------------------------------------------
ArgPair * SCFParser::argument()
{
	string name = next_token().lexeme;
	match(SCFTT_ID);
	match(SCFTT_EQUALS);
	DataType * dt = dataType();

	return new ArgPair(name, dt);
}

// ----------------------------------------------------------
// This function processes and returns a data type.
//
// Version 5.0
// ----------------------------------------------------------
DataType * SCFParser::dataType()
{
	SCFToken type = next_token();
	match(SCFTT_ID);

	if (type.lexeme == "double")
		return DataType::DT_DOUBLE;
	else if (type.lexeme == "string")
		return DataType::DT_STRING;
	else if (type.lexeme == "null")
		return DataType::DT_NULL;
	else
	{
		types->add(type.lexeme);
		return types->getDT(type.lexeme);
	}
}

// ----------------------------------------------------------
// This function determines if the given UDFRecord matches 
// the required PEF.
// @rec: The UDFRecord to compare to PEF.
//
// Version 5.0
// ----------------------------------------------------------
bool SCFParser::isPEF(UDFRecord * rec, string pefName)
{
	if (rec->returnType != DataType::DT_NULL)
		return false;

	if (rec->name != pefName)
		return false;

	if (rec->args->size() != 0)
		return false;

	return true;
}

// ----------------------------------------------------------
// This function determines if the given UDFRecord exists 
// in the functions list.
// @rec: The UDFRecord to compare to files.
// @functions: A list of functions
//
// Version 5.0
// ----------------------------------------------------------
bool SCFParser::isInFunctions(UDFRecord * rec, UDFCollection * functions)
{
	for (int recordIndex = 0; recordIndex < functions->size(); recordIndex++)
	{
		UDFRecord * currRec = functions->at(recordIndex);

		if (functionSignatureMatches(currRec, rec))
			return true;
	}

	return false;
}

// ----------------------------------------------------------
// This function determines if the given ObjectStruct exists 
// in the objects list.
// @rec: The UDFRecord to compare to files.
// @objects: A list of objects
//
// Version 5.0
// ----------------------------------------------------------
bool SCFParser::isInObjects(ObjectStruct * rec, OFCollection * objects)
{
	for (int objectIndex = 0; objectIndex < objects->size(); objectIndex++)
	{
		ObjectStruct * currRec = objects->at(objectIndex);

		if (rec->name == currRec->name)
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
// Version 5.0
// ----------------------------------------------------------
bool SCFParser::functionSignatureMatches(UDFRecord * first, UDFRecord * second)
{
	if (first->primary != second->primary)
		return false; 

	if (first->name != second->name)
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
// Version 5.0
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
		case SCFTT_LPAREN:
			type = SCFToken::LPAREN.lexeme;
			break;
		case SCFTT_RPAREN:
			type = SCFToken::RPAREN.lexeme;
			break;
		case SCFTT_EQUALS:
			type = SCFToken::EQUALS.lexeme;
			break;
		case SCFTT_COMMA:
			type = SCFToken::COMMA.lexeme;
			break;
		case SCFTT_TILDE:
			type = SCFToken::TILDE.lexeme;
			break;
		case SCFTT_DOT:
			type = SCFToken::DOT.lexeme;
			break;
		case SCFTT_SCANEOF:
			type = "<EOF>";
			break;
		case SCFTT_ID:
			type = "Identifier";
			break;
		case SCFTT_EOL:
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
void SCFParser::syntax_error(string err_msg)
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
SCFToken SCFParser::next_token()
{
	if (lookahead[0].type == SCFTT_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}

// ----------------------------------------------------------
// This function opens the SCF and checks for existance.
// @SCFPath: The path to the SCF.
//
// Version 5.0
// ----------------------------------------------------------
void SCFParser::open(string SCFPath)
{
	scanner.open(SCFPath);

	if (!scanner.good())
	{
		scanner.close();
		syntax_error("SCF not found: " + SCFPath);
	}

	currFilePath = SCFPath;
}