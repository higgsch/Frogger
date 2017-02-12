//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
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
// This function closes the input file stream.
//
// Version 3.0
// ----------------------------------------------------------
void Parser::close()
{
	scanner.close();
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
// <prog> => <flowstmt> <flowstmts> $
//
// Version 2.0
// ----------------------------------------------------------
void Parser::prog()
{
	ControlFlowNode* first = flowstmt(); 
	first->addNextStmt(flowstmts());
	root->addFirstStmt(first);
}

// ----------------------------------------------------------
// This function represents production rules:
// <flowstmts> => <flowstmt> <flowstmts>
// <flowstmts> => [lambda]
//
// Version 2.0
// ----------------------------------------------------------
ControlFlowNode* Parser::flowstmts()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case SCANEOF:
		//lambda
		return NULL;
		break;
	case IF:
	default:
		ControlFlowNode* thisStmt = flowstmt(); 
		thisStmt->addNextStmt(flowstmts());
		return thisStmt;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <flowstmt> => <ifstmt>
// <flowstmt> => <jmpstmt>
//
// Version 2.0
// ----------------------------------------------------------
ControlFlowNode* Parser::flowstmt()
{
	Token tok = next_token();
	switch(tok.type)
	{
	case IF:
		return ifstmt();
		break;
	default:
		return jmpstmt();
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <nestedflowstmt> => <ifstmt>
// <nestedflowstmt> => <jmpstmt>
//
// Version 2.0
// ----------------------------------------------------------
ControlFlowNode* Parser::nestedflowstmt()
{
	ControlFlowNode* stmt;

	Token tok = next_token();
	switch (tok.type)
	{
	case IF:
		stmt = ifstmt();
		break;
	default:
		stmt = jmpstmt();
		break;
	}

	stmt->setNested(true);
	return stmt;
}

// ----------------------------------------------------------
// This function represents production rules:
// <ifstmt> => if ( <boolexp> ) then <nestedflowstmt> else <nestedflowstmt>
//
// Version 2.0
// ----------------------------------------------------------
IfNode* Parser::ifstmt()
{
	match(IF); match(LPAREN);
	BinaryOpNode* toCompare = boolexp();
	match(RPAREN); match(THEN);
	ControlFlowNode* trueStmt = nestedflowstmt();
	match(ELSE);
	ControlFlowNode* falseStmt = nestedflowstmt();

	IfNode* stmt = new IfNode();
	stmt->setBoolExp(toCompare);
	stmt->setTrueStmt(trueStmt);
	stmt->setFalseStmt(falseStmt);
	return stmt;
}

// ----------------------------------------------------------
// This function represents production rules:
// <boolexp> => <expr> <boolop> <expr>
// <boolexp> => <expr> not <boolop> <expr>
//
// Version 2.0
// ----------------------------------------------------------
BinaryOpNode* Parser::boolexp()
{
	AbstractNode* left = expr(); 

	Token tok = next_token();
	BinaryOpNode* not = NULL;
	if (tok.type == NOT)
	{
		match(NOT); 
		not = new NotingNode();
	}

	BinaryOpNode* op = boolop(); 
	AbstractNode* right = expr();
	op->addOps(left, right);

	if (not == NULL)
	{
		return op;
	}
	else
	{
		not->addLeftChild(op);
		return not;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <boolop> => lt
// <boolop> => gt
// <boolop> => eq
// <boolop> => lte
// <boolop> => gte
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
// This function represents production rules:
// <jmpstmt> => <commandname> ( [<arglist>] ) ;
// <jmpstmt> => id assign <expr> ;
// Returns: A pointer to the node representing this jmpstmt.
//
// Version 2.5
// ----------------------------------------------------------
JmpStmtNode* Parser::jmpstmt()
{
	JmpStmtNode* stmt = new JmpStmtNode();

	Token idTok = next_token();
	match(ID);

	Token tok = next_token();
	switch (tok.type)
	{
	case LPAREN:
		{
			match(LPAREN);
			CommandCallNode* cmd = new CommandCallNode(idTok.lexeme);

			Token argTok = next_token();
			if (argTok.type != RPAREN)
			{
				cmd->addRightChild(arglist(0, cmd->getCmd()));
			}

			match(RPAREN);
			match(SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case ASSIGN:
		{
			IdRefNode* id = new IdRefNode(idTok.lexeme);
			match(ASSIGN);

			AbstractNode* toAssign = expr();
			match(SEMICOLON);

			stmt->setStmt(new AssigningNode(id, toAssign));
			break;
		}
	default:
		syntax_error("Invalid start of stmt - " + tok.lexeme);
		return NULL;
		break;
	}

	return stmt;
}

// ----------------------------------------------------------
// This function represents production rules:
// <arglist> => <expr> , <arglist>
// <arglist> => <expr>
// Returns: A pointer to the node representing this term.
//
// Version 2.4
// ----------------------------------------------------------
AbstractNode* Parser::arglist(int argNo, Command* cmd)
{
	AbstractNode * firstArg = expr();
	AbstractNode * nextArg = NULL;

	ArgListNode * list = new ArgListNode();
	list->setCmd(cmd);
	list->addLeftChild(firstArg);
	list->setArgNo(argNo);
	cmd->addArg(DT_NOT_DEFINED);
	argNo++;

	if (next_token().type == COMMA)
	{
		match(COMMA);
		nextArg = arglist(argNo, cmd);
	}

	list->addRightChild(nextArg);
	return list;
}

// ----------------------------------------------------------
// This function represents production rules:
// <expr> => <addterm> <expr.1>
// <expr.1> => <addop> <expr>
// <expr.1> => [lambda]
// Returns: A pointer to the node representing this value.
//
// Version 1.0
// ----------------------------------------------------------
AbstractNode* Parser::expr()
{
	AbstractNode* left = addterm();

	Token tok = next_token();
	switch(tok.type)
	{
	case ADD: //fall through
	case SUB:
		{
			BinaryOpNode* op = addop();
			AbstractNode* right = expr();
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
	case DIV: //fall through
	case MOD: //fall through
	case IDIV:
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
// <multerm> => <typedterm> <multerm.1>
// <multerm.1> => <expop> <multerm>
// <multerm.1> => [lambda]
// Returns: A pointer to the node representing this term.
//
// Version 2.1
// ----------------------------------------------------------
AbstractNode* Parser::multerm()
{
	AbstractNode* left = typedterm();

	Token tok = next_token();
	switch (tok.type)
	{
	case ROOT: //fall through
	case EXP: 
		{
			BinaryOpNode* op = expop();
			AbstractNode* right = multerm();
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
// <typedterm> => <primary> <typedterm.1>
// <typedterm.1> => : id ( <arglist> ) <typedterm.1>
// <typedterm.1> => : id ( ) <typedterm.1>
// <typedterm.1> => [lambda]
// Returns: A pointer to the node representing this term.
//
// Version 2.5
// ----------------------------------------------------------
AbstractNode* Parser::typedterm()
{
	AbstractNode * root = NULL;
	AbstractNode * prim = primary();
	AbstractNode * curr = prim;

	do
	{
		Token tok = next_token();
		if (tok.type == COLON)
		{
			match(COLON);

			Token idTok = next_token();
			match(ID);
			FunctionCallNode * funct = new FunctionCallNode(idTok.lexeme);
			match(LPAREN);

			Token firstArg = next_token();

			if (firstArg.type != RPAREN)
			{
				funct->addRightChild(arglist(0,funct->getFunct()));
			}

			match(RPAREN);

			funct->addLeftChild(curr);
			curr = funct;
		}
		else
		{
			root = curr;
		}
	} while (root == NULL);

	return root;
}

// ----------------------------------------------------------
// This function represents production rules:
// <primary> => dbl
// <primary> => id
// <primary> => id ( [<arglist>] )
// <primary> => string
// <primary> => ( <expr> )
// Returns: A pointer to the node representing this term.
//
// Version 2.5
// ----------------------------------------------------------
AbstractNode* Parser::primary()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case DOUBLECONST:
		match(DOUBLECONST);
		return new DoubleConstingNode(tok.lexeme);
		break;
	case ID:
		{
			match(ID);
		
			Token nextTok = next_token();
			if (nextTok.type != LPAREN)
				return new IdRefNode(tok.lexeme);

			//Function call
			FunctionCallNode * funct = new FunctionCallNode(tok.lexeme);
			match(LPAREN);

			Token firstArg = next_token();
			if (firstArg.type != RPAREN)
			{
				funct->addRightChild(arglist(0, funct->getFunct()));
			}

			match(RPAREN);
			return funct;
			break;
		}
	case STRING:
		match(STRING);
		return new StringConstingNode(tok.lexeme);
		break;
	case LPAREN:
		{
			match(LPAREN); AbstractNode* val = expr(); match(RPAREN);
			val->addParenNesting();
			return val;
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
// <mulop> => mod
// <mulop> => idiv
// Returns: A pointer to the node representing this operator.
//
// Version 2.1
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
	case MOD:
		match(tok.type);
		return new ModDivingNode();
		break;
	case IDIV:
		match(tok.type);
		return new IDivingNode();
		break;
	default:
		syntax_error("Expected '**', '//', '%%', or '\\\\' - Found " + tok.lexeme);
		return NULL;
		break;
	}
}

// ----------------------------------------------------------
// This function represents production rules:
// <expop> => rt
// <expop> => exp
// Returns: A pointer to the node representing this operator.
//
// Version 2.1
// ----------------------------------------------------------
BinaryOpNode* Parser::expop()
{
	Token tok = next_token();
	switch (tok.type)
	{
	case ROOT:
		match(tok.type);
		return new RootingNode();
		break;
	case EXP:
		match(tok.type);
		return new ExpingNode();
		break;
	default:
		syntax_error("Expected '##' or '^^' - Found " + tok.lexeme);
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
// Version 2.5
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
			type = "=";
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
		case MOD:
			type = "%%";
			break;
		case IDIV:
			type = "\\\\";
			break;
		case ROOT:
			type = "##";
			break;
		case EXP:
			type = "^^";
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
		case IF:
			type = "if";
			break;
		case THEN:
			type = "then";
			break;
		case ELSE:
			type = "else";
			break;
		case COLON:
			type = ":";
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