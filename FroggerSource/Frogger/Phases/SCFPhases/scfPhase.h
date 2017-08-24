// -----------------------------------------------------------------
// This is the header for the SCFPhase class.
// -----------------------------------------------------------------
#pragma once

#include <iostream>
#include "..\..\DataStructures\Tables\structs.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This class provides the base of the visitor inheritance for
// the SCF intermediate representation. 
//
// Version 5.0
// ----------------------------------------------------------
class SCFPhase
{
protected:
	// ----------------------------------------------------------
	// This function calls processOF for each OF in the given
	// Object.
	//
	// Version 5.0
	// ----------------------------------------------------------
	void runForLocalOFs(ObjectStruct * obj)
	{
		int objCount = obj->getNumberOfOFs();
		for (int objIndex = 0; objIndex < objCount; objIndex++)
		{
			processOF(obj->getOF(objIndex));
		}
	}

	// ----------------------------------------------------------
	// This function calls processUDF for each UDF in the given
	// Object.
	//
	// Version 5.0
	// ----------------------------------------------------------
	void runForLocalUDFs(ObjectStruct * obj)
	{
		int udfCount = obj->getNumberOfUDFs();
		for (int udfIndex = 0; udfIndex < udfCount; udfIndex++)
		{
			processUDF(obj->getUDF(udfIndex));
		}
	}

	// ----------------------------------------------------------
	// This function displays an error message to the user and 
	// terminates the program.
	// @err_msg: The message to display to the user.
	// 
	// Version 4.4
	// ----------------------------------------------------------
	void struct_error(string err_msg)
	{
		cout << "STRUCTURAL ERROR : " << err_msg << endl;

		if (!quietMode)
		{
			cout << "Press Enter to Exit" << endl;

			getchar();
		}
		exit(0);
	}

	virtual void processUDF(UDFRecord * udf) = 0;
	virtual void processOF(ObjectStruct * obj) = 0;

public:
	virtual void process(ProgramStruct * prog) = 0;
};