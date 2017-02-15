// -----------------------------------------------------------------
// This is the include header file for Node classes. 
// -----------------------------------------------------------------
#pragma once

#include "Nodes\node.h"
#include "Nodes\programNode.h"

#include "Nodes\ControlFlowNodes\controlFlowNode.h"
#include "Nodes\ControlFlowNodes\ifNode.h"
#include "Nodes\ControlFlowNodes\jmpStmtNode.h"

#include "Nodes\AsciiNodes\asciiNode.h"
#include "Nodes\AsciiNodes\unaryNode.h"

#include "Nodes\AsciiNodes\BinaryNodes\binaryNode.h"
#include "Nodes\AsciiNodes\BinaryNodes\assigningNode.h"
#include "Nodes\AsciiNodes\BinaryNodes\commandNodes.h"
#include "Nodes\AsciiNodes\BinaryNodes\functionCallNodes.h"

#include "Nodes\AsciiNodes\BinaryNodes\BinaryOpNodes\binaryOpNode.h"
#include "Nodes\AsciiNodes\BinaryNodes\BinaryOpNodes\logicNodes.h"
#include "Nodes\AsciiNodes\BinaryNodes\BinaryOpNodes\arithmeticNodes.h"

#include "Nodes\AsciiNodes\TerminalNodes\constingNodes.h"
#include "Nodes\AsciiNodes\TerminalNodes\idRefNode.h"
using namespace std;