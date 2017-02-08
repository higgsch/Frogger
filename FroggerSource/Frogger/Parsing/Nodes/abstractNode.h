// -----------------------------------------------------------------
// This is the header file for the AbstractNode class. 
// -----------------------------------------------------------------
#pragma once

#include "node.h"
using namespace std;

class Phase;

//Set of node data types
enum DataType
{
	DT_DOUBLE,DT_STRING,
	DT_NOT_DEFINED
};

// ----------------------------------------------------------
// This class provides the base of the node inheritance for
// the AST intermediate representation. Supports the visitor
// pattern for a Phase.
//
// Version 2.3
// ----------------------------------------------------------
class AbstractNode : public Node
{
protected:
	AbstractNode * parent; // this node's parent
	AbstractNode * leftChild; // this node's left child
	AbstractNode * rightChild; // this node's right child
	DataType dataType; // the node's data type
	node_type type; // the node category
	string lexeme; // the string representation of the node value
	node_side parent_side; // indicates which child (left or right)
	int ascii_addition; // the value this node contributes to the Stmt's ascii
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

	DataType getDataType() { return dataType; }
	void setDataType(DataType i_dataType) { dataType = i_dataType; }

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