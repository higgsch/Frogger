// -----------------------------------------------------------------
// This is the header for the FileExistencePhase class.
// -----------------------------------------------------------------
#pragma once

#include "scfPhase.h"
#include <string>
#include <fstream>
using namespace std;

// ----------------------------------------------------------
// This class checks for existence of referenced files. 
//
// Version 5.2
// ----------------------------------------------------------
class FileExistencePhase : SCFPhase
{
private:
	string dir;

	void verifyFileExists(string filename);
	void assignUDFFilename(UDFRecord * udf);

	string replaceAll(string source, string toFind, string toReplace)
	{
		size_t n = 0;
		while ((n = source.find(toFind, n)) != string::npos)
		{
			source = source.replace( n, toFind.size(), toReplace);
			n += toReplace.size();
		}

		return source;
	}

protected:
	void processUDF(UDFRecord * udf);
	void processOF(ObjectStruct * obj);

public:
	void process(ProgramStruct * prog);

	FileExistencePhase(string topRootDir) : dir(topRootDir) {}
};