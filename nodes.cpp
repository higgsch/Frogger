//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
// -----------------------------------------------------------------
// This program provides node representations for the AST.
// -----------------------------------------------------------------
#include "nodes.h"
#include "phases.h"
using namespace std;

// ----------------------------------------------------------
// Default constructor initializes pointers and counters.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode::AbstractNode()
{
	parent = NULL;
	leftChild = NULL;
	rightChild = NULL;
	parenNestCount = 0;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// left child.
// @absNode: The new left child for this node.
//
// Version 2.0
// ----------------------------------------------------------
void AbstractNode::addLeftChild(AbstractNode * absNode)
{
	if (absNode == NULL)
		return;
	if (leftChild == NULL)
	{
		absNode->setParent(this);
		absNode->setWhichSide(LEFT);
		leftChild = absNode;
	}
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function incorporates the given node as this node's
// right child.
// @absNode: The new right child for this node.
//
// Version 2.0
// ----------------------------------------------------------
void AbstractNode::addRightChild(AbstractNode * absNode)
{
	if (absNode == NULL)
		return;
	if (rightChild == NULL)
	{
		absNode->setParent(this);
		absNode->setWhichSide(RIGHT);
		rightChild = absNode;

	}
	else
		cout << "COMPILER ERROR: expression not correct" << endl;
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 1.0
// ----------------------------------------------------------
void AbstractNode::clean()
{
	if (leftChild != NULL)
	{
		leftChild->clean();
		free(leftChild);
	}
	if (rightChild != NULL)
	{
		rightChild->clean();
		free(rightChild);
	}
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
ProgramNode::ProgramNode()
{
	firstStmt = NULL;
	lineCount = 0;
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::clean()
{
	if (firstStmt != NULL)
	{
		firstStmt->clean();
		free(firstStmt);
	}
}

// ----------------------------------------------------------
// Adds a line of code's root node to the AST.
// @line: The line's root node.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::addLineNode(AbstractNode * line)
{
	if (firstStmt == NULL)
		firstStmt = new LineNode(0);

	firstStmt->addLine(line);

	lineCount++;
}

// ----------------------------------------------------------
// Adds an if statement's root node to the AST.
// @?: The line's root node.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::addIfNode(IfStruct ifStruct)
{
	if (firstStmt == NULL)
		firstStmt = new IfNode(0);

	firstStmt->addIf(ifStruct);

	lineCount++;
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::printNodes(ostream* out)
{
	cout << "Printing in Order" << endl;
	if (firstStmt == NULL)
		*out << "NULL";
	else
		firstStmt->printNodes(out);

	*out << endl << endl;
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void ProgramNode::traverseNodes(Phase* p)
{
	if (firstStmt != NULL)
		firstStmt->traverseNodes(p);
}

// ----------------------------------------------------------
// This constructor builds a node for an if statement.
// @lineNumber: The number identification associated to this
// line of code.
//
// Version 2.0
// ----------------------------------------------------------
IfNode::IfNode(int lineNumber)
{
	boolExp = NULL;
	trueLine = NULL;
	falseLine = NULL;
	nextStmt = NULL;
	lineNo = lineNumber;
}

// ----------------------------------------------------------
// Adds a line of code's root node to the AST.
// @addLine: The line's root node.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::addLine(AbstractNode * addLine)
{
	if (boolExp == NULL)
	{ // this IfNode is the final in the list
		//TODO error: empty if, trying to add line
		return;
	}

	if (nextStmt == NULL) 
		//this IfNode holds an if statement
		//and there is no next ControlFlowNode in the list
		nextStmt = new LineNode(lineNo + 1);

	nextStmt->addLine(addLine);
}

// ----------------------------------------------------------
// Adds an if statement's root node to the AST.
// @boolExpNode:	The if's comparison.
// @trueLineNode:	The if's line for true comparisons.
// @falseLineNode:	The if's line for false comparisons.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::addIf(IfStruct ifStruct)
{
	if (boolExp == NULL)
	{
		boolExp = ifStruct.boolExp;
		trueLine = ifStruct.trueLine;
		falseLine = ifStruct.falseLine;
		return;
	}

	if (nextStmt == NULL)
		//this IfNode holds an if statement
		//and there is no next ControlFlowNode in the list
		nextStmt = new IfNode(lineNo + 1);

	nextStmt->addIf(ifStruct);
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::clean()
{
	if (boolExp != NULL)
	{
		boolExp->clean();
		free(boolExp);
	}

	if (trueLine != NULL)
	{
		trueLine->clean();
		free(trueLine);
	}

	if (falseLine != NULL)
	{
		falseLine->clean();
		free(falseLine);
	}

	if (nextStmt != NULL)
	{
		nextStmt->clean();
		free(nextStmt);
	}
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::printNodes(ostream* out)
{
	if (boolExp == NULL)
		*out << "NULL";
	else
	{
		*out << "if ";
		boolExp->printMe(out);
		*out << "\n\t";
		trueLine->printNodes(out);
		*out << " else\n\t";
		falseLine->printNodes(out);
	}

	if (nextStmt == NULL)
		*out << "NULL";
	else
		nextStmt->printNodes(out);
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void IfNode::traverseNodes(Phase* p)
{
	if (boolExp != NULL)
	{
		p->visit(this);
	}
	if (nextStmt != NULL)
		nextStmt->traverseNodes(p);
}

// ----------------------------------------------------------
// This constructor builds a node for a line of code.
// @lineNumber: The number identification associated to this
// line of code.
//
// Version 2.0
// ----------------------------------------------------------
LineNode::LineNode(int lineNumber)
{
	line = NULL;
	nextStmt = NULL;
	lineNo = lineNumber;
}

// ----------------------------------------------------------
// Adds a line of code's root node to the AST.
// @addLine: The line's root node.
//
// Version 2.0
// ----------------------------------------------------------
void LineNode::addLine(AbstractNode * addLine)
{
	if (line == NULL)
	{ // this LineNode is the final in the list
		line = addLine;
		return;
	}

	if (nextStmt == NULL) 
		//this LineNode holds a line of code
		//and there is no next ControlFlowNode in the list
		nextStmt = new LineNode(lineNo + 1);

	nextStmt->addLine(addLine);
}

// ----------------------------------------------------------
// Adds an if statement's root node to the AST.
// @boolExpNode:	The if's comparison.
// @trueLineNode:	The if's line for true comparisons.
// @falseLineNode:	The if's line for false comparisons.
//
// Version 2.0
// ----------------------------------------------------------
void LineNode::addIf(IfStruct ifStruct)
{
	if (line == NULL)
	{ //TODO error: empty line, add if
		return;
	}

	if (nextStmt == NULL)
		//this LineNode holds a line of code
		//and there is no next ControlFlowNode in the list
		nextStmt = new IfNode(lineNo + 1);

	nextStmt->addIf(ifStruct);
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 2.0
// ----------------------------------------------------------
void LineNode::clean()
{
	if (line != NULL)
	{
		line->clean();
		free(line);
	}
	if (nextStmt != NULL)
	{
		nextStmt->clean();
		free(nextStmt);
	}
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void LineNode::printNodes(ostream* out)
{
	if (line == NULL)
		*out << "NULL";
	else if (lineNo != -1) // -1 is a flag for sub-statement lines
	{
		*out << "Line " << lineNo << ": ";
		line->printMe(out);
	}
	else
		line->printMe(out);

	*out << " Jump to: " << ascii_jmp << endl;

	if (lineNo != -1)
	{
		if (nextStmt == NULL)
			*out << "NULL";
		else
			nextStmt->printNodes(out);
	}
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 2.0
// ----------------------------------------------------------
void LineNode::traverseNodes(Phase* p)
{
	if (line != NULL)
	{
		p->visit(this);
	}
	if (lineNo != -1 && nextStmt != NULL)
		nextStmt->traverseNodes(p);
}

// ----------------------------------------------------------
// This constructor builds a node for a retrieve statement.
//
// Version 1.1
// ----------------------------------------------------------
RetrievalNode::RetrievalNode()
{
	type = RETRIEVING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.1
// ----------------------------------------------------------
void RetrievalNode::printMe(ostream* out)
{
	*out << "RETRIEVE";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.1
// ----------------------------------------------------------
void RetrievalNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This constructor builds a node for a display statement.
// @toDisplay: A node representing what should be displayed.
//
// Version 1.0
// ----------------------------------------------------------
DisplayingNode::DisplayingNode(AbstractNode* toDisplay)
{
	type = DISPLAYING;
	addLeftChild(toDisplay);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DisplayingNode::printMe(ostream* out)
{
	*out << "PRINT ";
	leftChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void DisplayingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 1.0
// ----------------------------------------------------------
EndingNode::EndingNode()
{
	type = ENDING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void EndingNode::printMe(ostream* out)
{
	*out << "END";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void EndingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This constructor builds a node for an assignment.
// @id: A node representing the variable to assign to.
// @toAssign: A node representing what to assign to the var.
//
// Version 1.0
// ----------------------------------------------------------
AssigningNode::AssigningNode(IdRefNode* id, AbstractNode* toAssign)
{
	type = ASSIGNING;
	addLeftChild(id);
	addRightChild(toAssign);
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AssigningNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " = ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void AssigningNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This constructor builds a node for a variable reference.
// @id: The variable's textual representation.
//
// Version 1.0
// ----------------------------------------------------------
IdRefNode::IdRefNode(string id)
{
	type = IDREF;
	lexeme = id;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void IdRefNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	*out << "ID(" << lexeme << ")";

	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void IdRefNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This constructor builds a node for a string constant.
// @toDisplay: The string's textual value.
//
// Version 1.0
// ----------------------------------------------------------
StringConstingNode::StringConstingNode(string str)
{
	type = STRINGCONSTING;
	lexeme = str;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void StringConstingNode::printMe(ostream* out)
{
	*out << "STR(" << lexeme << ")";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void StringConstingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This constructor builds a node for a double constant.
// @dbl: The textual representation of the double.
//
// Version 1.0
// ----------------------------------------------------------
DoubleConstingNode::DoubleConstingNode(string dbl)
{
	type = DBLCONSTING;
	lexeme = dbl;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DoubleConstingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	*out << "DBL(" << lexeme << ")";

	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void DoubleConstingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// This function adds the left and right operands to the AST.
// @left: The left side of the operator.
// @right: The right side of the operator.
//
// Version 1.0
// ----------------------------------------------------------
void BinaryOpNode::addOps(AbstractNode* left, AbstractNode* right)
{
	addLeftChild(left);
	addRightChild(right);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 1.0
// ----------------------------------------------------------
AddingNode::AddingNode()
{
	type = ADDING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void AddingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " + ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void AddingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 1.0
// ----------------------------------------------------------
SubingNode::SubingNode()
{
	type = SUBING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void SubingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " - ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void SubingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 1.0
// ----------------------------------------------------------
MulingNode::MulingNode()
{
	type = MULING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void MulingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " * ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void MulingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 1.0
// ----------------------------------------------------------
DivingNode::DivingNode()
{
	type = DIVING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void DivingNode::printMe(ostream* out)
{
	if (parenNestCount > 0)
		*out << "([" << parenNestCount << "] ";

	leftChild->printMe(out);
	*out << " / ";
	rightChild->printMe(out);
	
	if (parenNestCount > 0)
		*out << " )";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 1.0
// ----------------------------------------------------------
void DivingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
NotingNode::NotingNode()
{
	type = NOTING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void NotingNode::printMe(ostream* out)
{
	*out << " ! [";
	leftChild->printMe(out);
	*out << "] ";
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void NotingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
LTingNode::LTingNode()
{
	type = LTING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void LTingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " < ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void LTingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
GTingNode::GTingNode()
{
	type = GTING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void GTingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " > ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void GTingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
EQingNode::EQingNode()
{
	type = EQING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void EQingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " == ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void EQingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
LTEingNode::LTEingNode()
{
	type = LTEING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void LTEingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " <= ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void LTEingNode::accept(Phase* p)
{
	p->visit(this);
}

// ----------------------------------------------------------
// Default constructor.
//
// Version 2.0
// ----------------------------------------------------------
GTEingNode::GTEingNode()
{
	type = GTEING;
}

// ----------------------------------------------------------
// This function prints this node to the given output stream.
// @out: The stream to display to.
//
// Version 2.0
// ----------------------------------------------------------
void GTEingNode::printMe(ostream* out)
{
	leftChild->printMe(out);
	*out << " >= ";
	rightChild->printMe(out);
}

// ----------------------------------------------------------
// This function allows double dispatch required for visitor
// pattern.
// @p: The visitor to operate on this node.
//
// Version 2.0
// ----------------------------------------------------------
void GTEingNode::accept(Phase* p)
{
	p->visit(this);
}