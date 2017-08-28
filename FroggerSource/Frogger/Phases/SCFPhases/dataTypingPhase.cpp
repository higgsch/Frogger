//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program checks for existence of referenced files.
// -----------------------------------------------------------------
#include "dataTypingPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function merges all scoped tables within the given
// ObjectStruct into cumulative tables.
// @obj: The object to accumulate.
// 
// Version 5.0
// ----------------------------------------------------------
void DataTypingPhase::populateAllTables(ObjectStruct* obj)
{
	int objCount = obj->getNumberOfOFs();
	for (int objIndex = 0; objIndex < objCount; objIndex++)
	{
		populateAllTables(obj->getOF(objIndex));
	}

	allCommands->merge(obj->scopedTables->cmds);
	allFunctions->merge(obj->scopedTables->functs);
	allSymbols->merge(obj->scopedTables->syms);
}