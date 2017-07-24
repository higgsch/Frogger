//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.2
// -----------------------------------------------------------------
// This program compiles a Frogger source to c++ output.
// -----------------------------------------------------------------
#include <fstream>
#include "froggerC.h"
#include "fgrFunctionC.h"
#include "..\Parsing\SCFParser.h"
#include "..\Phases\phases.h"
#include <iostream>
using namespace std;

// ----------------------------------------------------------
// This function drives the PEFF compilation process.
// @inFile: The .fgr file to open (from project directory).
// @outFile: The file that output source is sent to.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::compileInputFile(string inFile, string outFile)
{
	progStruct.PEF->returnType = DT_NULL;

	int start = inFile.find_last_of('\\') + 1;

	int end = inFile.find_last_of('.');
	end = (end == -1) ? inFile.length() - 1 : end;

	progStruct.PEF->UDFName = inFile.substr(start, end - start);
	progStruct.PEF->args = new vector<argPair *>();

	FgrFunctionC funcComp(lang);
	progAST.PEF = funcComp.compileFunctionToAST(inFile, new FunctionTable(lang), progStruct.PEF);

	computeRequiredSupportCode(&progAST);
	
	emitInputFileCode(outFile);

	cout << "Program successfully compiled" << endl;
}

// ----------------------------------------------------------
// This function drives the PF compilation process.
// @inProject: The PF path.
// @outFile: The file that output source is sent to.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::compileInputProject(string projectDir, string projectName, string outFile)
{
	SCFParser p;
	progStruct.UDFs = p.parseSCF(projectDir + projectName + ".struct", projectName);
	FunctionTable * table = new FunctionTable(lang);

	//verify all referenced files exist
	int index = 0;
	while (index < progStruct.UDFs->size())
	{
		UDFRecord * currRec = (*progStruct.UDFs)[index];
		string currUDFPath = projectDir + currRec->UDFName + ".fgr";
		ifstream currUDF(currUDFPath);
		if (!currUDF.good())
		{
			struct_error(currUDFPath + " does not exist or is not accessible.");
		}
		
		currUDF.close();

		Function * currFunction = new Function(DT_NULL,currRec->UDFName,currRec->returnType);

		int argIndex = 0;
		while (argIndex < currRec->args->size())
		{
			currFunction->addArg((*(currRec->args))[argIndex]->type);
			argIndex++;
		}
		table->add(new FunctionRecord(currFunction));
		index++;
	}

	//compile PEFF
	progStruct.PEF->UDFName = projectName;
	progStruct.PEF->returnType = DT_NULL;
	progStruct.PEF->args = new vector<argPair *>();
	FgrFunctionC funcComp(lang);
	progAST.PEF = funcComp.compileFunctionToAST(projectDir + projectName + ".fgr", table, progStruct.PEF);

	//compile all referenced files
	index = 0;
	while (index < progStruct.UDFs->size())
	{
		progAST.UDFs->push_back(funcComp.compileFunctionToAST(projectDir + (*progStruct.UDFs)[index]->UDFName + ".fgr", table, (*progStruct.UDFs)[index]));
		index++;
	}

	computeRequiredSupportCode(&progAST);

	//emit project code
	emitInputProjectCode(outFile);

	cout << "Program successfully compiled" << endl;
}

// ----------------------------------------------------------
// This function drives support code requirements checking.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::computeRequiredSupportCode(ProgramAST * progAST)
{
	SupportReqsPhase * reqs = new SupportReqsPhase();
	reqs->gatherRequirements(lang, progAST);
	delete reqs;
}

// ----------------------------------------------------------
// This function drives code generation for a single file input.
// @outFile: The file that output source is sent to.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::emitInputFileCode(string outFile)
{
	CodeGenerationPhase *cgp = new CodeGenerationPhase(lang);

	cgp->open(outFile);
	cgp->printMetaCode(&progStruct);
	cgp->printPEFCode(progAST.PEF, progStruct.PEF);
	cgp->close();

	delete cgp;
}

// ----------------------------------------------------------
// This function drives code generation for a project input.
// @outFile: The file that output source is sent to.
//
// Version 4.2
// ----------------------------------------------------------
void FroggerC::emitInputProjectCode(string outFile)
{
	CodeGenerationPhase *cgp = new CodeGenerationPhase(lang);

	cgp->open(outFile);
	cgp->printMetaCode(&progStruct);
	cgp->printPEFCode(progAST.PEF,progStruct.PEF);

	int index = 0;
	while (index < progAST.UDFs->size())
	{
		cgp->printUDFCode((*(progAST.UDFs))[index], (*(progStruct.UDFs))[index]);
		index++;
	}

	cgp->close();

	delete cgp;
}

// ----------------------------------------------------------
// This function displays an error message to the user and 
// terminates the program.
// @err_msg: The message to display to the user.
// 
// Version 4.0
// ----------------------------------------------------------
void FroggerC::struct_error(string err_msg)
{
	cout << "STRUCTURAL ERROR : " << err_msg << endl;

	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}
}

//Forward declarations for main
void printUsage();
void printHelp();
string stripTrailingSlashes(string s);

// ----------------------------------------------------------
// The main function for the frogger compiler. Takes a .fgr 
// file and .cpp file from command line and runs the 
// compilation process.
// Note: Command line accepts .fgr filename then .cpp filename.
//
// Version 4.0
// ----------------------------------------------------------
int main(int argc, char* argv[])
{
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
			inPath = inPath + "\\" + projectName + ".fgr";
		}
		else
		{
			SCF.close();
			if (!quietMode)
				cout << "Starting Project Compilation: " << inPath << "\\ -> " << outFilePath << endl;
	
			FroggerC c;
			c.compileInputProject(inPath + "\\", projectName, outFilePath);
		}
	}

	if (inFile)
	{
		ifstream PEF(inPath);
		if (!PEF.good())
		{
			cout << "PEFF DNE: " << inPath << endl;
		}
		else
		{
			PEF.close();
			if (!quietMode)
			cout << "Starting File Compilation: " << inPath << " -> " << outFilePath << endl;
	
			FroggerC c;
			c.compileInputFile(inPath, outFilePath);
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