//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 2.0
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
// This function represents production rule:
// <prog> => <stmt> <stmts> $
//
// Version 2.0
// ----------------------------------------------------------
void Parser::prog()
{
	Token tok = next_token();
	if (tok.type == IF)
		root->addIfNode(ifstmt());
	else
		root->addLineNode(line());

	stmts();
}

// ----------------------------------------------------------
// This function represents production rules:
// <stmts> => <ifstmt> <stmts>
// <stmts> => <line> <stmts>
// <stmts> => [lambda]
//
// Version 2.0
// ----------------------------------------------------------
void Parser::stmts()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case SCANEOF:
		//lambda
		break;
	case IF:
		root->addIfNode(ifstmt()); stmts();
	default:
		root->addLineNode(line()); stmts();
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <ifstmt> -> if ( <boolexp> ) <line> else <line>
//
// Version 2.0
// ----------------------------------------------------------
IfStruct Parser::ifstmt()
{
	match(IF); match(LPAREN);
	BinaryOpNode* toCompare = boolexp();
	match(RPAREN);
	AbstractNode* trueAbs = line();
	match(ELSE);
	AbstractNode* falseAbs = line();

	IfStruct ifStruct;
	ifStruct.boolExp = toCompare;
	ifStruct.trueLine = new LineNode(-1);
	ifStruct.falseLine = new LineNode(-1);
	ifStruct.trueLine->addLine(trueAbs);
	ifStruct.falseLine->addLine(falseAbs);
	return ifStruct;
}

// ----------------------------------------------------------
// This function represents production rules:
// <boolexp> -> <dblval> <boolops> <dblval>
//
// Version 2.0
// ----------------------------------------------------------
BinaryOpNode* Parser::boolexp()
{
	AbstractNode* left = dblval(); 
	BinaryOpNode* op = boolops(); 
	AbstractNode* right = dblval();
	op->addOps(left, right);
	return op;
}

// ----------------------------------------------------------
// This function represents production rules:
// <line> => display ( <strval> ) ;
// <line> => display ( <dblval> ) ;
// <line> => end ;
// <line> => id assign <dblval> ;
// Returns: A pointer to the node representing this line.
//
// Version 2.0
// ----------------------------------------------------------
AbstractNode* Parser::line()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case DISPLAY:
		{
			match(DISPLAY); match(LPAREN);

			Token t = next_token();
			AbstractNode* toDisplay;
			if (t.type == STRING)
				toDisplay = strval();
			else
				toDisplay = dblval();

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
// This function represents production rules:
// <strval> => string
// Returns: A pointer to the node representing this value.
//
// Version 2.0
// ----------------------------------------------------------
AbstractNode* Parser::strval()
{
	Token tok = next_token();
	match(STRING);
	return new StringConstingNode(tok.lexeme);
}

// ----------------------------------------------------------
// This function represents production rules:
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
// This function represents production rules:
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
// This function represents production rules:
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
// This function represents production rules:
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
// This function represents production rules:
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
// This function represents production rules:
// <boolops> -> <boolop>
// <boolops> -> not <boolop>
// Returns: A pointer to the node representing this operator.
//
// Version 2.0
// ----------------------------------------------------------
BinaryOpNode* Parser::boolops()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case NOT:
		{
		match(NOT);
		BinaryOpNode* not = new NotingNode();
		not->addLeftChild(boolop());
		return not;
		break;
		}
	case LT:
	case GT:
	case EQ:
	case LTE:
	case GTE:
		return boolop();
		break;
	default:
		syntax_error("'" + tok.lexeme + "' - Invalid Boolean Operator");
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <boolop> -> lt
// <boolop> -> gt
// <boolop> -> eq
// <boolop> -> lte
// <boolop> -> gte
// Returns: A pointer to the node representing this operator.
//
// Version 2.0
// ----------------------------------------------------------
BinaryOpNode* Parser::boolop()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case LT:
		match(LT);
		return new LTingNode();
		break;
	case GT:
		match(GT);
		return new GTingNode();
		break;
	case EQ:
		match(EQ);
		return new EQingNode();
		break;
	case LTE:
		match(LTE);
		return new LTEingNode();
		break;
	case GTE:
		match(GTE);
		return new GTEingNode();
		break;
	default:
		syntax_error(lookahead[0].lexeme + " - Invalid Boolean Operator");
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
// Version 2.0
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
		case NOT:
			type = "!";
			break;
		case LT: 
			type = "<";
			break;
		case GT:
			type = ">";
			break;
		case EQ:
			type = "==";
			break;
		case LTE:
			type = "<=";
			break;
		case GTE:
			type = ">=";
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
		case IF:
			type = "if";
			break;
		case ELSE:
			type = "else";
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