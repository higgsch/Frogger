// -----------------------------------------------------------------
// This is the header file for the AssigningNode class. 
// -----------------------------------------------------------------
#pragma once

#include "binaryNode.h"
#include "..\TerminalNodes\idRefNode.h"
using namespace std;

class IdRefNode;

// ----------------------------------------------------------
// This class provides a node representation for the 
// assignment operation.
//
// Version 3.1
// ----------------------------------------------------------
class AssigningNode : public BinaryNode
{
public:
	AssigningNode(IdRefNode* id, AsciiNode* toAssign, int lineNo) 
		: BinaryNode(lineNo) {
		addLeftChild(id); addRightChild(toAssign); }
	~AssigningNode() {}

	void addAssignee(AsciiNode* n) { addLeftChild(n); }
	void addAssignor(AsciiNode* n) { addRightChild(n); }

	AsciiNode* getAssignee() { return getLeftChild(); }
	AsciiNode* getAssignor() { return getRightChild(); }

	void visitAssignee(FGRPhase* p) { visitLeftChild(p); }
	void visitAssignor(FGRPhase* p) { visitRightChild(p); }
	void accept(FGRPhase* p) { p->visit(this); }
};