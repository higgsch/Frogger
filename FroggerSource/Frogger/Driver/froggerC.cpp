//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program compiles a Frogger source to c++ output.
// -----------------------------------------------------------------
#include <fstream>
#include "froggerC.h"
#include "fgrFunctionC.h"
#include "..\Parsing\SCFParser.h"
#include "..\Phases\phases.h"
#include <iostream>
#include <Windows.h>
using namespace std;

string rootDir = "";

// ----------------------------------------------------------
// Default Constructor.
//
// Version 5.0
// ----------------------------------------------------------
FroggerC::FroggerC()
{ 
	lang = new CPPLanguage();
	lang->initialize();

	progStruct = new ProgramStruct(lang);
	p = new SCFParser(lang);
}

// ----------------------------------------------------------
// This function drives the compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 5.0
// ----------------------------------------------------------
void FroggerC::compile(string dir, string name, string outFile, bool toExe, bool cleanup, bool isProject)
{
	if (isProject)
	{
		progStruct = p->parseProgramLevelSCF(dir, name);
	}
	else
	{
		progStruct->PEF = new UDFRecord(DataType::DT_NULL, name, DataType::DT_NULL, lang);
		progStruct->PEF->visibleCmds->addEndNull();
		progStruct->scopedCmds->add(progStruct->PEF);
	}

	addAllLocalUDFsToVisibles();

	verifyPEFExists(dir);
	verifyAllContainedUDFsExist(dir, progStruct);
	
	progStruct->types = p->getTypeList();
	funcComp = new FgrFunctionC(lang, progStruct);

	compilePEF(dir);
	compileAllContainedUDFs(dir, progStruct);

	computeRequiredSupportCode(progStruct);

	emitCode(dir, name, outFile, toExe, cleanup, isProject);

	cout << "Program successfully compiled" << endl;
}

// ----------------------------------------------------------
// This function checks that the PEF exists.
// @dir: The folder in which the PEF should appear.
// 
// Version 5.0
// ----------------------------------------------------------
void FroggerC::verifyPEFExists(string dir)
{
	verifyFileExists(dir + getUDFFilename(progStruct->PEF));
}

// ----------------------------------------------------------
// This function checks that all files within the given 
// collection exist.
// @dir: The folder in which the udf files should appear.
// @obj: The object.
// 
// Version 5.0
// ----------------------------------------------------------
void FroggerC::verifyAllContainedUDFsExist(string dir, ObjectStruct * obj)
{
	int udfCount = obj->getNumberOfUDFs();
	for (int udfIndex = 0; udfIndex < udfCount; udfIndex++)
	{
		UDFRecord* currRec = obj->getUDF(udfIndex);
		verifyFileExists(dir + getUDFFilename(currRec));
	}

	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		ObjectStruct* currObj = obj->getOF(objIndex);
		if (currObj->isUserDefined)
		{
			verifyAllContainedUDFsExist(dir + currObj->name + "\\", currObj);
		}
	}
}

// ----------------------------------------------------------
// This function compiles the PEF.
// @dir: The folder in which the udf files should appear.
// 
// Version 5.0
// ----------------------------------------------------------
void FroggerC::compilePEF(string dir)
{
	funcComp->compileFunctionToAST(dir + getUDFFilename(progStruct->PEF), progStruct->PEF);
}

// ----------------------------------------------------------
// This function compiles all UDFs within the given collection.
// @dir: The folder in which the udf files should appear.
// @udfs: The collection of UDFs.
// @functs: The function table to use for compilation.
// @cmds: The command table to use for compilation.
// 
// Version 5.0
// ----------------------------------------------------------
void FroggerC::compileAllContainedUDFs(string dir, ObjectStruct * obj)
{
	int udfCount = obj->getNumberOfUDFs();
	for (int udfIndex = 0; udfIndex < udfCount; udfIndex++ )
	{
		UDFRecord * currUDF = obj->getUDF(udfIndex);
		string path = dir + getUDFFilename(currUDF);
		funcComp->compileFunctionToAST(path, currUDF);
	}

	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		ObjectStruct * currObj = obj->getOF(objIndex);
		if (currObj->isUserDefined)
		{
			compileAllContainedUDFs(dir + currObj->name + "\\", currObj);
		}
	}
}

// ----------------------------------------------------------
// This function merges all scoped tables into all visible tables.
//
// Version 5.0
// ----------------------------------------------------------
void FroggerC::addAllLocalUDFsToVisibles()
{
	addAllLocalUDFsToVisibles(progStruct);
}

// ----------------------------------------------------------
// This function merges all scoped tables into all visible tables.
// @obj: The working ObjectStruct.
//
// Version 5.0
// ----------------------------------------------------------
void FroggerC::addAllLocalUDFsToVisibles(ObjectStruct* obj)
{
	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		addAllLocalUDFsToVisibles(obj->getOF(objIndex));
	}

	int udfCount = obj->getNumberOfUDFs();
	for (int udfIndex = 0; udfIndex < udfCount; udfIndex++)
	{
		UDFRecord* currUDF = obj->getUDF(udfIndex);
		currUDF->visibleCmds->merge(obj->scopedCmds);
		currUDF->visibleFuncts->merge(obj->scopedFuncts);
		currUDF->visibleSyms->merge(obj->scopedSymbols);
	}
}

// ----------------------------------------------------------
// This function drives support code requirements checking.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::computeRequiredSupportCode(ProgramStruct * prog)
{
	SupportReqsPhase * reqs = new SupportReqsPhase();
	reqs->gatherRequirements(lang, prog);
	delete reqs;
}

// ----------------------------------------------------------
// This function drives code generation.
// @outFile: The file that output source is sent to.
//
// Version 5.0
// ----------------------------------------------------------
void FroggerC::emitCode(string dir, string name, string outFile, bool toExe, bool cleanup, bool isProject)
{
	string filename = (toExe) ? rootDir + "bin\\" + name : outFile;

	CodeGenerationPhase *cgp = new CodeGenerationPhase(lang);

	cgp->open(filename + ".cpp");
	cgp->printMetaCode(progStruct);
	cgp->printPEFCode(progStruct);
	cgp->printAllContainedUDFsCode(progStruct);
	cgp->close();

	if (toExe)
		cgp->outputToExe(filename, outFile);

	if (toExe && cleanup)
		cgp->cleanup(filename);

	delete cgp;
}

// ----------------------------------------------------------
// This function checks that the given file exists.
// @filename: The path to the file.
// 
// Version 5.0
// ----------------------------------------------------------
void FroggerC::verifyFileExists(string filename)
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
// Version 5.0
// ----------------------------------------------------------
string FroggerC::getUDFFilename(UDFRecord * udf)
{
	string filename = udf->name + "(";

	int argCount = udf->args->size();
	if (argCount > 0)
		filename += udf->args->at(0)->name + "=" + udf->args->at(0)->type->typeString;

	for (int argIndex = 1; argIndex < argCount; argIndex++)
	{
		ArgPair * currArg = udf->args->at(argIndex);
		filename += "," + currArg->name + "=" + currArg->type->typeString;
	}

	filename += ")~" + udf->returnType->typeString;

	return filename + ".fgr";
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @err_msg: The message to display to the user.
// 
// Version 4.4
// ----------------------------------------------------------
void FroggerC::struct_error(string err_msg)
{
	cout << "STRUCTURAL ERROR : " << err_msg << endl;

	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

//Forward declarations for main
void initializeRootDir();
void printUsage();
void printHelp();
string stripTrailingSlashes(string s);

// ----------------------------------------------------------
// The main function for the frogger compiler. Takes a .fgr 
// file and .cpp file from command line and runs the 
// compilation process.
// Note: Command line accepts .fgr filename then .cpp filename.
//
// Version 5.0
// ----------------------------------------------------------
int main(int argc, char* argv[])
{
	initializeRootDir();

	if (argc == 1)
	{
		printUsage();
		return 0;
	}

	quietMode = false;
	int argIndex = 1;
	string helpArg = argv[argIndex];
	if (helpArg == "/?")
	{
		printHelp();
		return 0;
	}

	string inPath = ""; //.fgr PEFF path for stand-alone function or PF path
	string outFilePath = ""; //.cpp filepath
	bool inFile = false;
	bool inProject = false;
	bool toExe = true;
	bool cleanup = true;

	bool error = false;

	while (argIndex < argc)
	{
		string switchArg = argv[argIndex];
		argIndex++;

		if (switchArg[0] != '/')
		{
			cout << "Invalid command: " << switchArg << "\n";
			error = true;
		}
		else if (switchArg == "/q")
		{
			quietMode = true;
			continue;
		}
		else if (switchArg == "/i")
		{
			toExe = false;
			continue;
		}
		else if (switchArg == "/v")
		{
			cleanup = false;
			continue;
		}
		else if (switchArg != "/o" && switchArg != "/if" && switchArg != "/ip")
		{
			cout << "Unrecognized switch: " << switchArg << "\n";
			error = true;
		}
		else if (argIndex >= argc)
		{
			cout << "Incomplete switch: " << switchArg << "\n";
			error = true;
		}

		if (error)
		{
			printUsage();
			return 0;
		}

		string switchVal = argv[argIndex];
		argIndex++;
		switchVal = stripTrailingSlashes(switchVal);
		if (switchVal == "")
		{
			cout << "Invalid value for switch " << switchArg << "\n";
			error = true;
		}

		if (switchArg == "/o")
		{
			if (outFilePath == "")
			{
				outFilePath = switchVal;
			}
			else
			{
				cout << "Duplicate output switch values\n";
				error = true;
			}
		}
		else if (switchArg == "/if")
		{
			if (inPath != "")
			{
				error = true;
				if (inProject)
					cout << "Cannot process both PEFF and PF input directives\n";
				if (inFile)
					cout << "Duplicate PEFF input directives\n";
			}
			else
			{
				inPath = switchVal;			
				inFile = true;
			}
		}
		else if (switchArg == "/ip")
		{
			if (inPath != "")
			{
				error = true;
				if (inFile)
					cout << "Cannot process both PEFF and PF input directives\n";
				if (inProject)
					cout << "Duplicate PF input directives\n";
			}
			else
			{
				inPath = switchVal;
				inProject = true;
			}
		}
		else
		{
			cout << "Unexpected behavior.\n";
			return 0;
		}

		if (error)
		{
			printUsage();
			return 0;
		}
	}

	if (outFilePath == "")
	{
		cout << "Output directive required.\n";
		printUsage();
		return 0;
	}
	if (!(inFile || inProject))
	{
		cout << "Input directive required.\n";
		printUsage();
		return 0;
	}

	if (inProject)
	{
		string projectName = inPath.substr(inPath.find_last_of('\\') + 1);
		ifstream SCF(inPath + "\\" + projectName + ".struct");
		if (!SCF.good())
		{
			inFile = true;
			inProject = false;
			inPath = inPath + "\\" + projectName;
		}
		else
		{
			SCF.close();
			if (!quietMode)
				cout << "Starting Project Compilation: " << inPath << "\\ -> " << outFilePath << endl;
	
			FroggerC c;
			c.compile(inPath + "\\", projectName, outFilePath, toExe, cleanup, inProject);
		}
	}

	if (inFile)
	{
		ifstream PEF(inPath + "()~null.fgr");
		if (!PEF.good())
		{
			cout << "PEFF DNE: " << inPath << endl;
		}
		else
		{
			PEF.close();
			if (!quietMode)
			cout << "Starting File Compilation: " << inPath << " -> " << outFilePath << endl;
	
			int start = inPath.find_last_of('\\') + 1;
			int end = inPath.find_last_of('.');
			end = (end == -1) ? inPath.length() : end;

			string inFileDir = inPath.substr(0, start);
			string inFilename = inPath.substr(start, end - start);

			FroggerC c;
			c.compile(inFileDir, inFilename, outFilePath, toExe, cleanup, inProject);
		}
	}

	if (!quietMode)
	{
		cout << "\nPress Enter to Exit" << endl;
		getchar();
	}

	return 0;
}

// ----------------------------------------------------------
// This function displays usage data to the user.
//
// Version 4.0
// ----------------------------------------------------------
void printUsage()
{
	if (!quietMode)
	{
		cout << endl;
		cout << "Type 'Frogger.exe /?' to display the HELP page" << endl;
		cout << "usage:" << endl;
		cout << "Frogger.exe /if <input filepath> /o <output filepath>" << endl;
		cout << "Frogger.exe /ip <input projectpath> /o <output filepath>" << endl;
		cout << endl;

		getchar();
	}
	else
	{
		cout << "Usage";
	}
}

// ----------------------------------------------------------
// This function displays the help page to the user.
//
// Version 4.0
// ----------------------------------------------------------
void printHelp()
{
	if (!quietMode)
	{
		cout << endl;
		cout << "HELP page: empty" << endl;
		cout << endl;

		getchar();
	}
	else
	{
		cout << "Help";
	}
}

// ----------------------------------------------------------
// This function strips off all '\\' from the given string.
//
// Version 4.0
// ----------------------------------------------------------
string stripTrailingSlashes(string s)
{
	while (s.find_last_of('\\') == s.length() - 1 && s != "")
	{
		s = s.substr(0,s.length() - 1);
	}

	return s;
}

// ----------------------------------------------------------
// This function obtains and sets the Frogger.exe directory
// from the Windows operating system.
//
// Version 4.3
// ----------------------------------------------------------
void initializeRootDir()
{
	TCHAR path[MAX_PATH];
	int size = GetModuleFileName(NULL, path, sizeof(path));

	rootDir = "";

	for (int i = 0; i < size; i++)
	{
		rootDir += (char)path[i];
	}
	
	rootDir = rootDir.substr(0, rootDir.find_last_of("\\"));
	rootDir = rootDir.substr(0, rootDir.find_last_of("\\") + 1);
}