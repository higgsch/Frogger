// -----------------------------------------------------------------
// This is the header file for Node classes. 
// -----------------------------------------------------------------
#pragma once

#include "Nodes\programNode.h"

#include "Nodes\AbstractNodeChild\assigningNode.h"
#include "Nodes\AbstractNodeChild\commandNodes.h"
#include "Nodes\AbstractNodeChild\constingNodes.h"
#include "Nodes\AbstractNodeChild\idRefNode.h"
#include "Nodes\AbstractNodeChild\functionCallNodes.h"

#include "Nodes\AbstractNodeChild\BinaryOpNodeChild\logicNodes.h"
#include "Nodes\AbstractNodeChild\BinaryOpNodeChild\arithmeticNodes.h"

#include "Nodes\ControlFlowNodeChild\ifNode.h"
#include "Nodes\ControlFlowNodeChild\jmpStmtNode.h"
using namespace std;