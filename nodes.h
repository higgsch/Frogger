// -----------------------------------------------------------------
// This is the header file for Node classes. 
// -----------------------------------------------------------------
#pragma once

#include <string>
#include <iostream>
using namespace std;

class Phase;

// identification for which binary child a node is.
typedef enum node_sides{
	LEFT, RIGHT
} node_side;

// set of node categories
typedef enum node_types{
	RETRIEVING, DISPLAYING, ENDING, ASSIGNING, IDREF, STRINGCONSTING, 
	DBLCONSTING, ADDING, SUBING, MULING, DIVING
} node_type;

// ----------------------------------------------------------
// This class provides the base of the node inheritance for
// the AST intermediate representation. Supports the visitor
// pattern for a Phase.
//
// Version 1.0
// ----------------------------------------------------------
class AbstractNode
{
protected:
	AbstractNode * parent; // this node's parent
	AbstractNode * leftChild; // this node's left child
	AbstractNode * rightChild; // this node's right child
	node_type type; // the node category
	string lexeme; // the string representation of the node value
	node_side parent_side; // indicates which child (left or right)
	int ascii_addition; // the value this node contributes to the line's ascii
	int parenNestCount; // the number of parens surrounding this node directly

public:
	AbstractNode();

	// ----------------------------------------------------------
	// This function sets this node's parent.
	// @par: The new parent for this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setParent(AbstractNode * par)	{	parent = par;			}

	// ----------------------------------------------------------
	// This function sets the side this node occupies in the
	// parent node.
	// @which: Side indicator
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setWhichSide(node_side which)	{	parent_side = which;	}

	// ----------------------------------------------------------
	// This function returns an indicator for which side of the
	// parent this node occupies.
	//
	// Version 1.0
	// ----------------------------------------------------------
	node_side getWhichSide()			{	return parent_side;		}

	// ----------------------------------------------------------
	// This function returns this node's parent.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AbstractNode * getParent()			{	return parent;			}

	// ----------------------------------------------------------
	// This function returns this node's left child.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AbstractNode * getLeftChild()		{	return leftChild;		}

	// ----------------------------------------------------------
	// This function returns this node's right child.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AbstractNode * getRightChild()		{	return rightChild;		}

	// ----------------------------------------------------------
	// This function returns this node's category.
	//
	// Version 1.0
	// ----------------------------------------------------------
	node_type getType()					{	return type;			}

	// ----------------------------------------------------------
	// This function returns the textual representation of this
	// node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	string getLexeme()					{	return lexeme;			}

	// ----------------------------------------------------------
	// This function sets the textual represenation of this node.
	// @lex: The new textual representation.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setLexeme(string lex)			{   lexeme = lex;			}

	// ----------------------------------------------------------
	// This function sets the ascii sum contribution of this node.
	// @ascii: The sum of ascii symbols represented by this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setAscii(int ascii)			{   ascii_addition = ascii; }

	// ----------------------------------------------------------
	// This function returns the ascii sum contribution of this 
	// node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int getAscii()						{	return ascii_addition;	}

	// ----------------------------------------------------------
	// This function increments the paren nesting count.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void addParenNesting() { parenNestCount++; }

	// ----------------------------------------------------------
	// This function returns the paren nesting count of this node.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int  getParenNesting() { return parenNestCount; }

	void addLeftChild(AbstractNode * absNode);
	void addRightChild(AbstractNode * absNode);

	void clean();

	//virtual functions
	virtual void accept(Phase*)=0;
	virtual void printMe(ostream*)=0;
};

// ----------------------------------------------------------
// This class provides a node representation for a binary
// operation; such as addition and division.
//
// Version 1.0
// ----------------------------------------------------------
class BinaryOpNode : public AbstractNode
{
public:
	void addOps(AbstractNode*, AbstractNode*);
};

// ----------------------------------------------------------
// This class provides a node for a line of code.
//
// Version 1.0
// ----------------------------------------------------------
class LineNode
{
private:
	AbstractNode * line; // the AST for this line of code
	LineNode * nextLine; // the next line of code
	int lineNo; // an identifier for this line 
	//(not the line in source but the count of LINE from the cfg)

	int ascii_jmp; // the line to jump to after this line executes

public:
	LineNode(int);

	// ----------------------------------------------------------
	// This function sets the line to jump to after this line 
	// executes.
	// @num: The lineNo to jump to.
	//
	// Version 1.0
	// ----------------------------------------------------------
	void setJump(int num) { ascii_jmp = num; }

	// ----------------------------------------------------------
	// This function returns the line to jump to after this line
	// executes.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int  getJump() { return ascii_jmp; }

	// ----------------------------------------------------------
	// This function returns this line's line identifier.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int	 getLineNo() { return lineNo; }

	// ----------------------------------------------------------
	// This function returns the AST for this line of code.
	//
	// Version 1.0
	// ----------------------------------------------------------
	AbstractNode * getLine() { return line; }

	// ----------------------------------------------------------
	// This function returns the next line of code.
	//
	// Version 1.0
	// ----------------------------------------------------------
	LineNode * getNextLine() { return nextLine; }

	void addLine(AbstractNode * line);
	void clean();
	void printNodes(ostream* out);
	void traverseNodes(Phase* p);
};

// ----------------------------------------------------------
// This class provides a root node for the AST.
//
// Version 1.0
// ----------------------------------------------------------
class ProgramNode
{
private:
	LineNode * firstLine; // the first line of code in this program
	int lineCount; // the total number of lines within the program

public:
	ProgramNode();

	// ----------------------------------------------------------
	// This function returns the first line of code.
	//
	// Version 1.0
	// ----------------------------------------------------------
	LineNode* getFirstLine() { return firstLine; }

	// ----------------------------------------------------------
	// This function returns the number of lines in this program.
	//
	// Version 1.0
	// ----------------------------------------------------------
	int getLineCount() { return lineCount; }

	void addLineNode(AbstractNode * line);
	void clean();
	void printNodes(ostream* out);
	void traverseNodes(Phase* p);
};

// ----------------------------------------------------------
// This class provides a node representation for getting
// double values from the user.
//
// Version 1.1
// ----------------------------------------------------------
class RetrievalNode : public AbstractNode
{
public:
	RetrievalNode();
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for the display
// statement.
//
// Version 1.0
// ----------------------------------------------------------
class DisplayingNode : public AbstractNode
{
public:
	DisplayingNode(AbstractNode*);
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for the end
// statement.
//
// Version 1.0
// ----------------------------------------------------------
class EndingNode : public AbstractNode
{
public:
	EndingNode();
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a variable
// reference.
//
// Version 1.0
// ----------------------------------------------------------
class IdRefNode : public AbstractNode
{
public:
	IdRefNode(string);
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for the 
// assignment operation.
//
// Version 1.0
// ----------------------------------------------------------
class AssigningNode : public AbstractNode
{
public:
	AssigningNode(IdRefNode*, AbstractNode*);
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a string
// constant.
//
// Version 1.0
// ----------------------------------------------------------
class StringConstingNode : public AbstractNode
{
public:
	StringConstingNode(string);
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a double
// constant.
//
// Version 1.0
// ----------------------------------------------------------
class DoubleConstingNode : public AbstractNode
{
public:
	DoubleConstingNode(string);
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for an addition
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class AddingNode : public BinaryOpNode
{
public:
	AddingNode();
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a subtraction
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class SubingNode : public BinaryOpNode
{
public:
	SubingNode();
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a multiplication
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class MulingNode : public BinaryOpNode
{
public:
	MulingNode();
	void printMe(ostream*);
	void accept(Phase*);
};

// ----------------------------------------------------------
// This class provides a node representation for a division
// operation.
//
// Version 1.0
// ----------------------------------------------------------
class DivingNode : public BinaryOpNode
{
public:
	DivingNode();
	void printMe(ostream*);
	void accept(Phase*);
};