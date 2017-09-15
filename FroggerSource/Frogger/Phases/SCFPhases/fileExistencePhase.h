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
// Version 5.3
// ----------------------------------------------------------
class FileExistencePhase : SCFPhase
{
private:
	string dir;

	void verifyFileExists(string filename);
	void assignUDFFilename(UDFRecord * udf);

protected:
	void processUDF(UDFRecord * udf);
	void processOF(ObjectStruct * obj);

public:
	void process(ProgramStruct * prog);

	FileExistencePhase(string topRootDir) : dir(topRootDir) {}
};