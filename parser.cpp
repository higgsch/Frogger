//                      Christopher Higgs
//                      CS 6820 - 7:30 am
//                      Final Project
//                      Dr. Rague
//                      Due: 12/10/16
//                      Version: 1.2
// -----------------------------------------------------------------
// This program parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code. 
// -----------------------------------------------------------------
#include "parser.h"
using namespace std;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 1.0
// ----------------------------------------------------------
Parser::Parser()
{
	current_token = Token::NOTOK;
	lookahead[0] = Token::NOTOK;
	root = new ProgramNode();
}

// ----------------------------------------------------------
// This function opens the input file.
// @inFile: The .fgr file to open.
// Note: File reference from project directory.
//
// Version 1.1
// ----------------------------------------------------------
void Parser::open(string inFile)
{
	scanner.open(inFile);
	scanner.checkForObfuscation();
}

// ----------------------------------------------------------
// This function initiates the parse and returns a pointer
// to the root node in the AST.
//
// Version 1.0
// ----------------------------------------------------------
ProgramNode* Parser::parse()
{
	prog();
	cout << "Program successfully parsed" << endl;
	return root;
}

// ----------------------------------------------------------
// This function represents production rule #1:
// <prog> => <line> <lines> $
//
// Version 1.0
// ----------------------------------------------------------
void Parser::prog()
{
	root->addLineNode(line()); lines();
}

// ----------------------------------------------------------
// This function represents production rules #2 and 3:
// <lines> => <line> <lines>
// <lines> => [lambda]
//
// Version 1.0
// ----------------------------------------------------------
void Parser::lines()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case SCANEOF:
		//lambda
		break;
	default:
		root->addLineNode(line()); lines();
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #4, 5, and 6:
// <line> => display ( <val> ) ;
// <line> => end ;
// <line> => id assign <dblval> ;
// Returns: A pointer to the node representing this line.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode* Parser::line()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case DISPLAY:
		{
			match(DISPLAY); match(LPAREN); 
			AbstractNode* toDisplay = val(); 
			match(RPAREN); match(SEMICOLON);
			return new DisplayingNode(toDisplay);
			break;
		}
	case END:
		match(END); match(SEMICOLON);
		return new EndingNode();
		break;
	case ID:
		{
			match(ID); 
			IdRefNode* id = new IdRefNode(tok.lexeme);
			match(ASSIGN); 
			AbstractNode* toAssign = dblval(); 
			match(SEMICOLON);
			return new AssigningNode(id, toAssign);
			break;
		}
	default:
		syntax_error("Invalid start of line - " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #7 and 8:
// <val> => string
// <val> => <dblval>
// Returns: A pointer to the node representing this value.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode* Parser::val()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case STRING:
		match(STRING);
		return new StringConstingNode(tok.lexeme);
		break;
	case DOUBLECONST:
	case ID:
	case LPAREN:
		return dblval();
		break;
	default:
		syntax_error("Invalid display sequence - " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #9, 10.1, and 10.2:
// <dblval> => <addterm> <dblval.1>
// <dblval.1> => <addop> <dblval>
// <dblval.1> => [lambda]
// Returns: A pointer to the node representing this value.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode* Parser::dblval()
{
	AbstractNode* left = addterm();

	Token tok = next_token();
	switch(tok.type)
	{
	case ADD: //fall through
	case SUB:
		{
			BinaryOpNode* op = addop();
			AbstractNode* right = dblval();
			op->addOps(left, right);
			return op;
			break;
		}
	default:
		//lambda
		return left;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #11, 12.1, and 12.2:
// <addterm> => <multerm> <addterm.1>
// <addterm.1> => <mulop> <addterm>
// <addterm.1> => [lambda]
// Returns: A pointer to the node representing this term.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode* Parser::addterm()
{
	AbstractNode* left = multerm();

	Token tok = next_token();
	switch (tok.type)
	{
	case MUL: //fall through
	case DIV:
		{
			BinaryOpNode* op = mulop();
			AbstractNode* right = addterm();
			op->addOps(left, right);
			return op;
			break;
		}
	default:
		//lambda
		return left;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #13, 14, 15, and 16:
// <multerm> => dbl
// <multerm> => id
// <multerm> => ( <dblval> )
// <multerm> => retrieve ( )
// Returns: A pointer to the node representing this term.
//
// Version 1.2
// ----------------------------------------------------------
AbstractNode* Parser::multerm()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case DOUBLECONST:
		match(tok.type);
		return new DoubleConstingNode(tok.lexeme);
		break;
	case ID:
		match(tok.type);
		return new IdRefNode(tok.lexeme);
		break;
	case LPAREN:
		{
			match(LPAREN); AbstractNode* val = dblval(); match(RPAREN);
			val->addParenNesting();
			return val;
			break;
		}
	case RETRIEVE:
		{
			match(RETRIEVE); match(LPAREN); match(RPAREN);
			return new RetrievalNode();
			break;
		}
	default:
		syntax_error("Invalid Term - " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #17 and 18:
// <addop> => add
// <addop> => sub
// Returns: A pointer to the node representing this operator.
//
// Version 1.0
// ----------------------------------------------------------
BinaryOpNode* Parser::addop()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case ADD:
		match(tok.type);
		return new AddingNode();
		break;
	case SUB:
		match(tok.type);
		return new SubingNode();
		break;
	default:
		syntax_error("Expected '++' or '--' - Found " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules #19 and 20:
// <mulop> => mul
// <mulop> => div
// Returns: A pointer to the node representing this operator.
//
// Version 1.0
// ----------------------------------------------------------
BinaryOpNode* Parser::mulop()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case MUL:
		match(tok.type);
		return new MulingNode();
		break;
	case DIV:
		match(tok.type);
		return new DivingNode();
		break;
	default:
		syntax_error("Expected '**' or '//' - Found " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function tests if the next token matches toMatch and
// moves to the next token on success. It displayes an error
// on failure.
// @toMatch: The expected token category.
//
// Version 1.2
// ----------------------------------------------------------
void Parser::match(token_type toMatch)
{
	Token tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = Token::NOTOK;
	}
	else
	{
		string type;
		switch (toMatch)
		{
		case ASSIGN:
			type = "==";
			break;
		case ADD:
			type = "++";
			break;
		case SUB:
			type = "--";
			break;
		case MUL:
			type = "**";
			break;
		case DIV:
			type = "//";
			break;
		case RETRIEVE:
			type = "retrieve";
			break;
		case END:
			type = "end";
			break;
		case DISPLAY:
			type = "display";
			break;
		case SEMICOLON:
			type = ";";
			break;
		case LPAREN:
			type = "(";
			break;
		case RPAREN:
			type = ")";
			break;
		case SCANEOF:
			type = "<EOF>";
			break;
		case STRING:
			type = "String Literal";
			break;
		case ID:
			type = "Identifier";
			break;
		case DOUBLECONST:
			type = "Double Literal";
			break;
		default:
			type = "<type>";
			break;
		}

		syntax_error("Expected \'" + type + "\' - Found " + lookahead[0].lexeme);
	}
}

// ----------------------------------------------------------
// This function displays an error to the user and terminates
// the program.
// @err_msg: The message to display.
//
// Version 1.0
// ----------------------------------------------------------
void Parser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "SYNTAX ERROR on line " << lineNo << ": " << err_msg << endl;
	cout << "Press Enter to Exit" << endl;

	getchar();
	exit(0);
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 1.0
// ----------------------------------------------------------
Token Parser::next_token()
{
	if (lookahead[0].type == NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}

//int main(int argc, char * argv[])
//{
//	string inFile;
//	string outFile;
//
//	if (argc > 2)
//	{
//		inFile = argv[1];
//		outFile = argv[2];
//	}
//	else
//	{
//		inFile = "Sample1.fgr";
//		outFile = "Sample1.cpp";
//	}
//
//	cout << "Starting Parse: " << inFile << " -> " << outFile << endl;
//
//	Parser p = Parser();
//	p.open(inFile, outFile);
//
//	ProgramNode* root = p.parse();
//
//	root->printNodes(&cout);
//
//	cout << "Press Enter to exit" << endl;
//	getchar();
//
//	return 0;
//}