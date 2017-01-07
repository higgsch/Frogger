//                      Christopher Higgs
//                      CS 6820 - 7:30 am
//                      Final Project
//                      Dr. Rague
//                      Due: 12/10/16
//                      Version: 1.1
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
// Version 1.0
// ----------------------------------------------------------
void AbstractNode::addLeftChild(AbstractNode * absNode)
{
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
// Version 1.0
// ----------------------------------------------------------
void AbstractNode::addRightChild(AbstractNode * absNode)
{
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
// Version 1.0
// ----------------------------------------------------------
ProgramNode::ProgramNode()
{
	firstLine = NULL;
	lineCount = 0;
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 1.0
// ----------------------------------------------------------
void ProgramNode::clean()
{
	if (firstLine != NULL)
	{
		firstLine->clean();
		free(firstLine);
	}
}

// ----------------------------------------------------------
// Adds a line of code's root node to the AST.
// @line: The line's root node.
//
// Version 1.0
// ----------------------------------------------------------
void ProgramNode::addLineNode(AbstractNode * line)
{
	if (firstLine == NULL)
		firstLine = new LineNode(0);

	firstLine->addLine(line);

	lineCount++;
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void ProgramNode::printNodes(ostream* out)
{
	cout << "Printing in Order" << endl;
	if (firstLine == NULL)
		*out << "NULL";
	else
		firstLine->printNodes(out);

	*out << endl << endl;
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 1.0
// ----------------------------------------------------------
void ProgramNode::traverseNodes(Phase* p)
{
	if (firstLine != NULL)
		firstLine->traverseNodes(p);
}

// ----------------------------------------------------------
// This constructor builds a node for a line of code.
// @lineNumber: The number identification associated to this
// line of code.
//
// Version 1.0
// ----------------------------------------------------------
LineNode::LineNode(int lineNumber)
{
	line = NULL;
	nextLine = NULL;
	lineNo = lineNumber;
}

// ----------------------------------------------------------
// Adds a line of code's root node to the AST.
// @addLine: The line's root node.
//
// Version 1.0
// ----------------------------------------------------------
void LineNode::addLine(AbstractNode * addLine)
{
	if (line == NULL)
	{ // this LineNode is the final in the list
		line = addLine;
		return;
	}

	if (nextLine == NULL) 
		//this LineNode holds a line of code
		//and there is no next LineNode in the list
		nextLine = new LineNode(lineNo + 1);

	nextLine->addLine(addLine);
}

// ----------------------------------------------------------
// This function handles the memory cleanup.
//
// Version 1.0
// ----------------------------------------------------------
void LineNode::clean()
{
	if (line != NULL)
	{
		line->clean();
		free(line);
	}
	if (nextLine != NULL)
	{
		nextLine->clean();
		free(nextLine);
	}
}

// ----------------------------------------------------------
// Prints the in order AST traversal to the output stream.
// @out: The stream to display to.
//
// Version 1.0
// ----------------------------------------------------------
void LineNode::printNodes(ostream* out)
{
	if (line == NULL)
		*out << "NULL";
	else
	{
		*out << "Line " << lineNo << ": ";
		line->printMe(out);
	}

	*out << " Jump to: " << ascii_jmp << endl;

	if (nextLine == NULL)
		*out << "NULL";
	else
		nextLine->printNodes(out);
}

// ----------------------------------------------------------
// This function starts double dispatch required for visitor 
// pattern.
// @p: The visitor to run over the AST.
//
// Version 1.0
// ----------------------------------------------------------
void LineNode::traverseNodes(Phase* p)
{
	if (line != NULL)
	{
		p->visit(this);
	}
	if (nextLine != NULL)
		nextLine->traverseNodes(p);
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