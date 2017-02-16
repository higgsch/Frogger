// -----------------------------------------------------------------
// This is the include header file for Node classes. 
// -----------------------------------------------------------------
#pragma once

#include "node.h"
#include "programNode.h"

#include "ControlFlowNodes\controlFlowNode.h"
#include "ControlFlowNodes\ifNode.h"
#include "ControlFlowNodes\jmpStmtNode.h"

#include "AsciiNodes\asciiNode.h"
#include "AsciiNodes\unaryNode.h"

#include "AsciiNodes\BinaryNodes\binaryNode.h"
#include "AsciiNodes\BinaryNodes\assigningNode.h"
#include "AsciiNodes\BinaryNodes\commandNodes.h"
#include "AsciiNodes\BinaryNodes\functionCallNodes.h"

#include "AsciiNodes\BinaryNodes\BinaryOpNodes\binaryOpNode.h"
#include "AsciiNodes\BinaryNodes\BinaryOpNodes\logicNodes.h"
#include "AsciiNodes\BinaryNodes\BinaryOpNodes\arithmeticNodes.h"

#include "AsciiNodes\TerminalNodes\constingNodes.h"
#include "AsciiNodes\TerminalNodes\idRefNode.h"
using namespace std;