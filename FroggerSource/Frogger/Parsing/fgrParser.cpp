//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 4.0
// -----------------------------------------------------------------
// This program parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code. 
// -----------------------------------------------------------------
#include "fgrParser.h"
using namespace std;

extern bool quietMode;

// ----------------------------------------------------------
// This is the default constructor.
//
// Version 4.0
// ----------------------------------------------------------
FGRParser::FGRParser()
{
	current_token = FGRToken::NOTOK;
	lookahead[0] = FGRToken::NOTOK;
	root = new ProgramNode();
	currFileName = "";
}

// ----------------------------------------------------------
// This function opens the input file.
// @inFile: The .fgr file to open.
// Note: File reference from project directory.
//
// Version 4.0
// ----------------------------------------------------------
void FGRParser::open(string inFile)
{
	currFileName = inFile;
	scanner.openAndInitialize(inFile);
}

// ----------------------------------------------------------
// This function closes the input file stream.
//
// Version 4.0
// ----------------------------------------------------------
void FGRParser::close()
{
	currFileName = "";
	scanner.closeAndTerminate();
}

// ----------------------------------------------------------
// This function initiates the parse and returns a pointer
// to the root node in the AST.
//
// Version 4.0
// ----------------------------------------------------------
ProgramNode* FGRParser::parse()
{
	prog();
	return root;
}

// ----------------------------------------------------------
// This function represents production rule:
// <prog> => <flowstmt> <flowstmts> $
//
// Version 3.0
// ----------------------------------------------------------
void FGRParser::prog()
{
	FGRToken tok = next_token();
	if (tok.type == TOKTYPE_SCANEOF)
		this->syntax_error("A program must have at least one statement");

	ControlFlowNode* first = flowstmt(); 
	first->addNextStmt(flowstmts());
	root->addFirstStmt(first);
}

// ----------------------------------------------------------
// This function represents production rules:
// <flowstmts> => <flowstmt> <flowstmts>
// <flowstmts> => [lambda]
//
// Version 3.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::flowstmts()
{
	FGRToken tok = next_token();
	switch(tok.type)
	{
	case TOKTYPE_SCANEOF:
		//lambda
		return NULL;
		break;
	case TOKTYPE_IF:
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
// Version 3.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::flowstmt()
{
	FGRToken tok = next_token();
	switch(tok.type)
	{
	case TOKTYPE_IF:
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
// Version 3.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::nestedflowstmt()
{
	ControlFlowNode* stmt;

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_IF:
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
// Version 3.0
// ----------------------------------------------------------
IfNode* FGRParser::ifstmt()
{
	match(TOKTYPE_IF); match(TOKTYPE_LPAREN);
	AsciiNode* toCompare = boolexp();
	match(TOKTYPE_RPAREN); match(TOKTYPE_THEN);
	ControlFlowNode* trueStmt = nestedflowstmt();
	match(TOKTYPE_ELSE);
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
// Version 3.1
// ----------------------------------------------------------
AsciiNode* FGRParser::boolexp()
{
	AsciiNode* left = expr(); 

	FGRToken tok = next_token();

	NotingNode* not = NULL;
	if (tok.type == TOKTYPE_NOT)
	{
		match(TOKTYPE_NOT); 
		not = new NotingNode(scanner.getLineNo());
	}

	BinaryOpNode* op = boolop(); 
	AsciiNode* right = expr();
	op->addOps(left, right);

	if (not == NULL)
	{
		return op;
	}
	else
	{
		not->addOperand(op);
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
// Version 3.1
// ----------------------------------------------------------
BinaryOpNode* FGRParser::boolop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_LT:
		match(TOKTYPE_LT);
		return new LTingNode(scanner.getLineNo());
		break;
	case TOKTYPE_GT:
		match(TOKTYPE_GT);
		return new GTingNode(scanner.getLineNo());
		break;
	case TOKTYPE_EQ:
		match(TOKTYPE_EQ);
		return new EQingNode(scanner.getLineNo());
		break;
	case TOKTYPE_LTE:
		match(TOKTYPE_LTE);
		return new LTEingNode(scanner.getLineNo());
		break;
	case TOKTYPE_GTE:
		match(TOKTYPE_GTE);
		return new GTEingNode(scanner.getLineNo());
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
// Version 3.1
// ----------------------------------------------------------
JmpStmtNode* FGRParser::jmpstmt()
{
	JmpStmtNode* stmt = new JmpStmtNode();

	FGRToken idTok = next_token();
	match(TOKTYPE_ID);

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_LPAREN:
		{
			match(TOKTYPE_LPAREN);
			CommandCallNode* cmd = new CommandCallNode(idTok.lexeme, scanner.getLineNo());

			FGRToken argTok = next_token();
			if (argTok.type != TOKTYPE_RPAREN)
			{
				cmd->addArgList(arglist(0, cmd->getCmd()));
			}

			match(TOKTYPE_RPAREN);
			match(TOKTYPE_SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case TOKTYPE_ASSIGN:
		{
			IdRefNode* id = new IdRefNode(idTok.lexeme, scanner.getLineNo());
			match(TOKTYPE_ASSIGN);

			AsciiNode* toAssign = expr();
			match(TOKTYPE_SEMICOLON);

			stmt->setStmt(new AssigningNode(id, toAssign, scanner.getLineNo()));
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
// Version 3.1
// ----------------------------------------------------------
AsciiNode* FGRParser::arglist(int argNo, Command* cmd)
{
	AsciiNode * firstArg = expr();
	AsciiNode * nextArg = NULL;

	ArgListNode * list = new ArgListNode(scanner.getLineNo());
	list->setCmd(cmd);
	list->addThisArg(firstArg);
	list->setArgNo(argNo);
	cmd->addArg(DT_NOT_DEFINED);
	argNo++;

	if (next_token().type == TOKTYPE_COMMA)
	{
		match(TOKTYPE_COMMA);
		nextArg = arglist(argNo, cmd);
	}

	list->addNextArg(nextArg);
	return list;
}

// ----------------------------------------------------------
// This function represents production rules:
// <expr> => <addterm> <expr.1>
// <expr.1> => <addop> <expr>
// <expr.1> => [lambda]
// Returns: A pointer to the node representing this value.
//
// Version 3.0
// ----------------------------------------------------------
AsciiNode* FGRParser::expr()
{
	AsciiNode* left = addterm();

	FGRToken tok = next_token();
	switch(tok.type)
	{
	case TOKTYPE_ADD: //fall through
	case TOKTYPE_SUB:
		{
			BinaryOpNode* op = addop();
			AsciiNode* right = expr();
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
// Version 3.0
// ----------------------------------------------------------
AsciiNode* FGRParser::addterm()
{
	AsciiNode* left = multerm();

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_MUL: //fall through
	case TOKTYPE_DIV: //fall through
	case TOKTYPE_MOD: //fall through
	case TOKTYPE_IDIV:
		{
			BinaryOpNode* op = mulop();
			AsciiNode* right = addterm();
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
// Version 3.0
// ----------------------------------------------------------
AsciiNode* FGRParser::multerm()
{
	AsciiNode* left = typedterm();

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_ROOT: //fall through
	case TOKTYPE_EXP: 
		{
			BinaryOpNode* op = expop();
			AsciiNode* right = multerm();
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
// Version 3.1
// ----------------------------------------------------------
AsciiNode* FGRParser::typedterm()
{
	AsciiNode * root = NULL;
	AsciiNode * prim = primary();
	AsciiNode * curr = prim;

	do
	{
		FGRToken tok = next_token();
		if (tok.type == TOKTYPE_COLON)
		{
			match(TOKTYPE_COLON);

			FGRToken idTok = next_token();
			match(TOKTYPE_ID);
			FunctionCallNode * funct = new FunctionCallNode(idTok.lexeme, scanner.getLineNo());
			match(TOKTYPE_LPAREN);

			FGRToken firstArg = next_token();

			if (firstArg.type != TOKTYPE_RPAREN)
			{
				funct->addArgList(arglist(0,funct->getFunct()));
			}

			match(TOKTYPE_RPAREN);

			funct->addPrimary(curr);
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
// Version 3.1
// ----------------------------------------------------------
AsciiNode* FGRParser::primary()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_DOUBLECONST:
		match(TOKTYPE_DOUBLECONST);
		return new DoubleConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case TOKTYPE_ID:
		{
			match(TOKTYPE_ID);
		
			FGRToken nextTok = next_token();
			if (nextTok.type != TOKTYPE_LPAREN)
				return new IdRefNode(tok.lexeme, scanner.getLineNo());

			//Function call
			FunctionCallNode * funct = new FunctionCallNode(tok.lexeme, scanner.getLineNo());
			match(TOKTYPE_LPAREN);

			FGRToken firstArg = next_token();
			if (firstArg.type != TOKTYPE_RPAREN)
			{
				funct->addArgList(arglist(0, funct->getFunct()));
			}

			match(TOKTYPE_RPAREN);
			return funct;
			break;
		}
	case TOKTYPE_STRING:
		match(TOKTYPE_STRING);
		return new StringConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case TOKTYPE_LPAREN:
		{
			match(TOKTYPE_LPAREN); 
			AsciiNode* val = expr(); 
			match(TOKTYPE_RPAREN);
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
// Version 3.1
// ----------------------------------------------------------
BinaryOpNode* FGRParser::addop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_ADD:
		match(tok.type);
		return new AddingNode(scanner.getLineNo());
		break;
	case TOKTYPE_SUB:
		match(tok.type);
		return new SubingNode(scanner.getLineNo());
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
// Version 3.1
// ----------------------------------------------------------
BinaryOpNode* FGRParser::mulop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_MUL:
		match(TOKTYPE_MUL);
		return new MulingNode(scanner.getLineNo());
		break;
	case TOKTYPE_DIV:
		match(TOKTYPE_DIV);
		return new DivingNode(scanner.getLineNo());
		break;
	case TOKTYPE_MOD:
		match(TOKTYPE_MOD);
		return new ModDivingNode(scanner.getLineNo());
		break;
	case TOKTYPE_IDIV:
		match(TOKTYPE_IDIV);
		return new IDivingNode(scanner.getLineNo());
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
// Version 3.1
// ----------------------------------------------------------
BinaryOpNode* FGRParser::expop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case TOKTYPE_ROOT:
		match(TOKTYPE_ROOT);
		return new RootingNode(scanner.getLineNo());
		break;
	case TOKTYPE_EXP:
		match(TOKTYPE_EXP);
		return new ExpingNode(scanner.getLineNo());
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
// Version 3.0
// ----------------------------------------------------------
void FGRParser::match(fgr_token_type toMatch)
{
	FGRToken tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = FGRToken::NOTOK;
	}
	else
	{
		string type;
		switch (toMatch)
		{
		case TOKTYPE_ASSIGN:
			type = "=";
			break;
		case TOKTYPE_ADD:
			type = "++";
			break;
		case TOKTYPE_SUB:
			type = "--";
			break;
		case TOKTYPE_MUL:
			type = "**";
			break;
		case TOKTYPE_DIV:
			type = "//";
			break;
		case TOKTYPE_MOD:
			type = "%%";
			break;
		case TOKTYPE_IDIV:
			type = "\\\\";
			break;
		case TOKTYPE_ROOT:
			type = "##";
			break;
		case TOKTYPE_EXP:
			type = "^^";
			break;
		case TOKTYPE_NOT:
			type = "!";
			break;
		case TOKTYPE_LT: 
			type = "<";
			break;
		case TOKTYPE_GT:
			type = ">";
			break;
		case TOKTYPE_EQ:
			type = "==";
			break;
		case TOKTYPE_LTE:
			type = "<=";
			break;
		case TOKTYPE_GTE:
			type = ">=";
			break;
		case TOKTYPE_IF:
			type = "if";
			break;
		case TOKTYPE_THEN:
			type = "then";
			break;
		case TOKTYPE_ELSE:
			type = "else";
			break;
		case TOKTYPE_COLON:
			type = ":";
			break;
		case TOKTYPE_SEMICOLON:
			type = ";";
			break;
		case TOKTYPE_LPAREN:
			type = "(";
			break;
		case TOKTYPE_RPAREN:
			type = ")";
			break;
		case TOKTYPE_SCANEOF:
			type = "<EOF>";
			break;
		case TOKTYPE_STRING:
			type = "String Literal";
			break;
		case TOKTYPE_ID:
			type = "Identifier";
			break;
		case TOKTYPE_DOUBLECONST:
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
// Version 4.0
// ----------------------------------------------------------
void FGRParser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "SYNTAX ERROR in file " << currFileName << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
		exit(0);
	}
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 3.0
// ----------------------------------------------------------
FGRToken FGRParser::next_token()
{
	if (lookahead[0].type == TOKTYPE_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}