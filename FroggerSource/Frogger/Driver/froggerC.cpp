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
// This function drives the PEFF compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 4.4
// ----------------------------------------------------------
void FroggerC::compileInputFile(string fileDir, string inFilename, string outFile, bool toExe, bool cleanup)
{
	progStruct.PEF->returnType = DT_NULL;
	progStruct.PEF->UDFName = inFilename;

	FgrFunctionC funcComp(lang);

	CommandTable * cmdTable = new CommandTable(lang);
	Command * PEF = new Command(DT_NULL, progStruct.PEF->UDFName, false);
	cmdTable->add(new CommandRecord(PEF));

	funcComp.compileFunctionToAST(fileDir + getUDFFilename(progStruct.PEF), new FunctionTable(lang), cmdTable, progStruct.PEF);

	computeRequiredSupportCode(&progStruct);
	
	emitInputFileCode(fileDir, inFilename, outFile, toExe, cleanup);

	cout << "Program successfully compiled" << endl;
}

// ----------------------------------------------------------
// This function drives the PF compilation process.
// @inProject: The PF path.
// @outFile: The file that output source is sent to.
//
// Version 5.0
// ----------------------------------------------------------
void FroggerC::compileInputProject(string projectDir, string projectName, string outFile, bool toExe, bool cleanup)
{
	SCFParser p;
	progStruct = *p.parseProgramLevelSCF(projectDir, projectName);
	FunctionTable * functTable = new FunctionTable(lang);
	CommandTable * cmdTable = new CommandTable(lang);

	int numFiles = progStruct.UDFs->size();

	//verify all referenced files exist
	for (int index = 0; index < numFiles; index++)
	{
		UDFRecord currRec = *(progStruct[index]);
		string currUDFPath = projectDir + getUDFFilename(&currRec);
		ifstream currUDF(currUDFPath);
		if (!currUDF.good())
		{
			struct_error(currUDFPath + " does not exist or is not accessible.");
		}
		
		currUDF.close();

		if (currRec.returnType == DT_NULL)
		{
			Command * currCommand = new Command(DT_NULL, currRec.UDFName, false);

			int numArgs = currRec.args->size();
			for (int argIndex = 0; argIndex < numArgs; argIndex++)
			{
				currCommand->addArg("", currRec[argIndex]->type);
			}
			cmdTable->add(new CommandRecord(currCommand));
		}
		else
		{
			Function * currFunction = new Function(DT_NULL,currRec.UDFName,currRec.returnType,false);

			int numArgs = currRec.args->size();
			for (int argIndex = 0; argIndex < numArgs; argIndex++)
			{
				currFunction->addArg("", currRec[argIndex]->type);
			}
			functTable->add(new FunctionRecord(currFunction));
		}
	}

	//compile PEFF
	progStruct.PEF->UDFName = projectName;
	progStruct.PEF->returnType = DT_NULL;
	FgrFunctionC funcComp(lang);
	funcComp.compileFunctionToAST(projectDir + getUDFFilename(progStruct.PEF), functTable, cmdTable, progStruct.PEF);

	//compile all referenced files
	for (int index = 0; index < numFiles; index++ )
	{
		funcComp.compileFunctionToAST(projectDir + getUDFFilename(progStruct[index]), functTable, cmdTable, progStruct[index]);
	}

	computeRequiredSupportCode(&progStruct);

	//emit project code
	emitInputProjectCode(projectDir, projectName, outFile, toExe, cleanup);

	cout << "Program successfully compiled" << endl;
}

// ----------------------------------------------------------
// This function returns the filename corresponding to the 
// given UDFRecord.
// @udf: The UDFRecord in question.
//
// Version 4.4
// ----------------------------------------------------------
string FroggerC::getUDFFilename(UDFRecord * udf)
{
	string filename = udf->UDFName + "(";

	int argCount = udf->args->size();
	if (argCount > 0)
		filename += udf->args->at(0)->name + "=" + getDataTypeString(udf->args->at(0)->type);

	for (int argIndex = 1; argIndex < argCount; argIndex++)
	{
		ArgPair * currArg = udf->args->at(argIndex);
		filename += "," + currArg->name + "=" + getDataTypeString(currArg->type);
	}

	filename += ")~" + getDataTypeString(udf->returnType);

	return filename + ".fgr";
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
// This function drives code generation for a single file input.
// @outFile: The file that output source is sent to.
//
// Version 4.3
// ----------------------------------------------------------
void FroggerC::emitInputFileCode(string fileDir, string inFilename, string outFile, bool toExe, bool cleanup)
{
	string filename = (toExe) ? rootDir + "bin\\" + inFilename : outFile;

	CodeGenerationPhase *cgp = new CodeGenerationPhase(lang);

	cgp->open(filename + ".cpp");
	cgp->printMetaCode(&progStruct);
	cgp->printPEFCode(progStruct.PEF);
	cgp->close();

	if (toExe)
		cgp->outputToExe(filename, outFile);

	if (cleanup)
		cgp->cleanup(filename);

	delete cgp;
}

// ----------------------------------------------------------
// This function drives code generation for a project input.
// @outFile: The file that output source is sent to.
//
// Version 4.3
// ----------------------------------------------------------
void FroggerC::emitInputProjectCode(string projectDir, string projectName, string outFile, bool toExe, bool cleanup)
{
	string filename = (toExe) ? rootDir + "bin\\" + projectName : outFile;

	CodeGenerationPhase *cgp = new CodeGenerationPhase(lang);

	cgp->open(filename + ".cpp");
	cgp->printMetaCode(&progStruct);
	cgp->printPEFCode(progStruct.PEF);

	int numUDFs = progStruct.getNumberOfUDFs();
	for (int index = 0; index < numUDFs; index++)
	{
		cgp->printUDFCode(progStruct[index]);
	}

	cgp->close();

	if (toExe)
		cgp->outputToExe(filename, outFile);

	if (toExe && cleanup)
		cgp->cleanup(filename);

	delete cgp;
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
// Version 4.4
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
			inPath = inPath + "\\" + projectName;
		}
		else
		{
			SCF.close();
			if (!quietMode)
				cout << "Starting Project Compilation: " << inPath << "\\ -> " << outFilePath << endl;
	
			FroggerC c;
			c.compileInputProject(inPath + "\\", projectName, outFilePath, toExe, cleanup);
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
			c.compileInputFile(inFileDir, inFilename, outFilePath, toExe, cleanup);
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