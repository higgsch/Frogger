//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.2
// -----------------------------------------------------------------
// This program checks for existence of referenced files.
// -----------------------------------------------------------------
#include "fileExistencePhase.h"
using namespace std;

// ----------------------------------------------------------
// This function processes the Program.
// 
// Version 5.0
// ----------------------------------------------------------
void FileExistencePhase::process(ProgramStruct * prog)
{
	string topRootDir = dir;
	dir = dir + prog->name + "\\";

	processUDF(prog->PEF);

	dir = topRootDir;

	processOF(prog);
}

// ----------------------------------------------------------
// This function processes the given Object.
// 
// Version 5.0
// ----------------------------------------------------------
void FileExistencePhase::processOF(ObjectStruct * obj)
{
	if (obj->isUserDefined)
	{
		string rootDir = dir;
		dir = dir + obj->name + "\\";

		runForLocalUDFs(obj);

		runForLocalOFs(obj);

		dir = rootDir;
	}
}

// ----------------------------------------------------------
// This function processes the given UDF.
// 
// Version 5.0
// ----------------------------------------------------------
void FileExistencePhase::processUDF(UDFRecord * udf) 
{
	assignUDFFilename(udf);
	verifyFileExists(udf->filepath);
}

// ----------------------------------------------------------
// This function checks that the given file exists.
// @filename: The path to the file.
// 
// Version 5.0
// ----------------------------------------------------------
void FileExistencePhase::verifyFileExists(string filename)
{
	ifstream file(filename);
	if (!file.good())
	{
		struct_error(filename + " does not exist or is not accessible.");
	}
		
	file.close();
}

// ----------------------------------------------------------
// This function returns the filename corresponding to the 
// given UDFRecord.
// @udf: The UDFRecord in question.
//
// Version 5.2
// ----------------------------------------------------------
void FileExistencePhase::assignUDFFilename(UDFRecord * udf)
{
	string filename = udf->name + "(";

	int argCount = udf->args->size();
	if (argCount > 0)
	{
		string argType = udf->args->at(0)->type->typeString;
		filename += udf->args->at(0)->name + "=" + argType;
	}

	for (int argIndex = 1; argIndex < argCount; argIndex++)
	{
		ArgPair * currArg = udf->args->at(argIndex);
		string argType = currArg->type->typeString;
		filename += "," + currArg->name + "=" + argType;
	}

	string returnType = udf->returnType->typeString;
	DataTypeCollection * templatizers = udf->returnType->templatizers;
	if (templatizers != NULL && templatizers->size() > 0)
	{
		returnType += "%" + templatizers->at(0)->typeString;

		int tCount = templatizers->size();
		for (int tIndex = 1; tCount < tIndex; tIndex++)
		{
			returnType += "," + templatizers->at(tIndex)->typeString;
		}
		
		returnType += "%";
	}
	
	filename += ")~" + returnType;

	udf->filepath = dir + filename + ".fgr";
}