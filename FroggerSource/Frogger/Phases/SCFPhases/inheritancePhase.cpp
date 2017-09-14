//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.1
// -----------------------------------------------------------------
// This program facilitates inheritance checks.
// -----------------------------------------------------------------
#include "inheritancePhase.h"
using namespace std;

// ----------------------------------------------------------
// This function processes the Program.
// 
// Version 5.1
// ----------------------------------------------------------
void InheritancePhase::process(ProgramStruct * prog)
{
	root = prog;

	processOF(prog);
}

// ----------------------------------------------------------
// This function processes the given Object.
// 
// Version 5.1
// ----------------------------------------------------------
void InheritancePhase::processOF(ObjectStruct * obj)
{
	if (obj->isUserDefined)
	{
		if (obj->parentName != "")
		{
			ObjectStruct * parent = root->getObject(obj->parentName);
			if (parent == NULL)
				struct_error("Parent: '" + obj->parentName + "' is not defined");

			obj->parent = parent;
			obj->scopedTables->merge(parent->scopedTables);
		}

		runForLocalOFs(obj);
	}
}