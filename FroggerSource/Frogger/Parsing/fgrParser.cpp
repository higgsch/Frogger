//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
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
// Version 5.0
// ----------------------------------------------------------
FGRParser::FGRParser()
{
	current_token = FGRToken::NOTOK;
	lookahead[0] = FGRToken::NOTOK;
	lookahead[1] = FGRToken::NOTOK;
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
// Version 5.0
// ----------------------------------------------------------
void FGRParser::prog()
{
	FGRToken tok = next_token();
	if (tok.type == FGRTT_SCANEOF)
		syntax_error("A program must have at least one statement");

	ControlFlowNode* first = flowstmt(); 
	first->addNextStmt(flowstmts());
	root->addFirstStmt(first);
}

// ----------------------------------------------------------
// This function represents production rules:
// <flowstmts> => <flowstmt> <flowstmts>
// <flowstmts> => [lambda]
//
// Version 5.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::flowstmts()
{
	FGRToken tok = next_token();
	switch(tok.type)
	{
	case FGRTT_SCANEOF:
		//lambda
		return NULL;
		break;
	case FGRTT_IF:
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
// Version 5.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::flowstmt()
{
	FGRToken tok = next_token();
	switch(tok.type)
	{
	case FGRTT_IF:
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
// Version 5.0
// ----------------------------------------------------------
ControlFlowNode* FGRParser::nestedflowstmt()
{
	ControlFlowNode* stmt;

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_IF:
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
// Version 5.0
// ----------------------------------------------------------
IfNode* FGRParser::ifstmt()
{
	match(FGRTT_IF); match(FGRTT_LPAREN);
	AsciiNode* toCompare = boolexp();
	match(FGRTT_RPAREN); match(FGRTT_THEN);
	ControlFlowNode* trueStmt = nestedflowstmt();
	match(FGRTT_ELSE);
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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::boolexp()
{
	AsciiNode* left = expr(); 

	FGRToken tok = next_token();

	NotingNode* not = NULL;
	if (tok.type == FGRTT_NOT)
	{
		match(FGRTT_NOT); 
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
// Version 5.0
// ----------------------------------------------------------
BinaryOpNode* FGRParser::boolop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_LT:
		match(FGRTT_LT);
		return new LTingNode(scanner.getLineNo());
		break;
	case FGRTT_GT:
		match(FGRTT_GT);
		return new GTingNode(scanner.getLineNo());
		break;
	case FGRTT_EQ:
		match(FGRTT_EQ);
		return new EQingNode(scanner.getLineNo());
		break;
	case FGRTT_LTE:
		match(FGRTT_LTE);
		return new LTEingNode(scanner.getLineNo());
		break;
	case FGRTT_GTE:
		match(FGRTT_GTE);
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
// <jmpstmt> => <typedterm> ;       // NOTE: <typedterm> contains ': <commandname> ( [<arglist>] )'
// <jmpstmt> => <commandname> ( [<arglist>] ) ;
// <jmpstmt> => id assign <expr> ;
// Returns: A pointer to the node representing this jmpstmt.
//
// Version 5.0
// ----------------------------------------------------------
JmpStmtNode* FGRParser::jmpstmt()
{
	JmpStmtNode* stmt = new JmpStmtNode();

	FGRToken idTok = next_token();
	FGRToken tok = second_token();

	switch (tok.type)
	{
	case FGRTT_COLON:
		{
			AsciiNode * cmd = typedterm(false);
			match(FGRTT_SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case FGRTT_LPAREN:
		{
			match(FGRTT_ID);
			match(FGRTT_LPAREN);
			CommandCallNode* cmd = new CommandCallNode(DataType::DT_NULL, idTok.lexeme, scanner.getLineNo());

			FGRToken argTok = next_token();
			if (argTok.type != FGRTT_RPAREN)
			{
				cmd->addArgList(arglist(0, cmd->getCmd()));
			}

			match(FGRTT_RPAREN);
			match(FGRTT_SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case FGRTT_ASSIGN:
		{
			match(FGRTT_ID);
			IdRefNode* id = new IdRefNode(idTok.lexeme, scanner.getLineNo());
			match(FGRTT_ASSIGN);

			AsciiNode* toAssign = expr();
			match(FGRTT_SEMICOLON);

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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::arglist(int argNo, Routine* rout)
{
	AsciiNode * firstArg = expr();
	AsciiNode * nextArg = NULL;

	ArgListNode * list = new ArgListNode(scanner.getLineNo());
	list->setRoutine(rout);
	list->addThisArg(firstArg);
	list->setArgNo(argNo);
	rout->addArg("", DataType::DT_NOT_DEFINED);
	argNo++;

	if (next_token().type == FGRTT_COMMA)
	{
		match(FGRTT_COMMA);
		nextArg = arglist(argNo, rout);
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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::expr()
{
	AsciiNode* left = addterm();

	FGRToken tok = next_token();
	switch(tok.type)
	{
	case FGRTT_ADD: //fall through
	case FGRTT_SUB:
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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::addterm()
{
	AsciiNode* left = multerm();

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_MUL: //fall through
	case FGRTT_DIV: //fall through
	case FGRTT_MOD: //fall through
	case FGRTT_IDIV:
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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::multerm()
{
	AsciiNode* left = typedterm(true);

	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_ROOT: //fall through
	case FGRTT_EXP: 
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
// @isEndFunction: Triggers the final tail as function or command
//
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::typedterm(bool isEndFunction)
{
	AsciiNode * root = NULL;
	AsciiNode * prim = primary();
	AsciiNode * curr = prim;
	FunctionCallNode * endRoutine = NULL;

	do
	{
		FGRToken tok = next_token();
		if (tok.type == FGRTT_COLON)
		{
			match(FGRTT_COLON);

			FGRToken idTok = next_token();
			match(FGRTT_ID);
			FunctionCallNode * funct = new FunctionCallNode(idTok.lexeme, scanner.getLineNo());
			match(FGRTT_LPAREN);

			FGRToken firstArg = next_token();

			if (firstArg.type != FGRTT_RPAREN)
			{
				funct->addArgList(arglist(0,funct->getFunct()));
			}

			match(FGRTT_RPAREN);

			funct->addPrimary(curr);
			curr = funct;
			endRoutine = funct;
		}
		else
		{
			if (!isEndFunction)
			{
				CommandCallNode * cmd = new CommandCallNode(DataType::DT_NOT_DEFINED, endRoutine->getLexeme(), endRoutine->getLineNo());
				cmd->addArgList(endRoutine->getArgList());
				cmd->addPrimary(endRoutine->getPrimary());
				curr = cmd;

				//TODO
				//delete endRoutine;  //Deletes primary node: tell node to drop children?
			}

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
// Version 5.0
// ----------------------------------------------------------
AsciiNode* FGRParser::primary()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_DOUBLECONST:
		match(FGRTT_DOUBLECONST);
		return new DoubleConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case FGRTT_ID:
		{
			match(FGRTT_ID);
		
			FGRToken nextTok = next_token();
			if (nextTok.type != FGRTT_LPAREN)
				return new IdRefNode(tok.lexeme, scanner.getLineNo());

			//Function call
			FunctionCallNode * funct = new FunctionCallNode(tok.lexeme, scanner.getLineNo());
			funct->getFunct()->primary = DataType::DT_NULL;
			match(FGRTT_LPAREN);

			FGRToken firstArg = next_token();
			if (firstArg.type != FGRTT_RPAREN)
			{
				funct->addArgList(arglist(0, funct->getFunct()));
			}

			match(FGRTT_RPAREN);
			return funct;
			break;
		}
	case FGRTT_STRING:
		match(FGRTT_STRING);
		return new StringConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case FGRTT_LPAREN:
		{
			match(FGRTT_LPAREN); 
			AsciiNode* val = expr(); 
			match(FGRTT_RPAREN);
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
// Version 5.0
// ----------------------------------------------------------
BinaryOpNode* FGRParser::addop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_ADD:
		match(tok.type);
		return new AddingNode(scanner.getLineNo());
		break;
	case FGRTT_SUB:
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
// Version 5.0
// ----------------------------------------------------------
BinaryOpNode* FGRParser::mulop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_MUL:
		match(FGRTT_MUL);
		return new MulingNode(scanner.getLineNo());
		break;
	case FGRTT_DIV:
		match(FGRTT_DIV);
		return new DivingNode(scanner.getLineNo());
		break;
	case FGRTT_MOD:
		match(FGRTT_MOD);
		return new ModDivingNode(scanner.getLineNo());
		break;
	case FGRTT_IDIV:
		match(FGRTT_IDIV);
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
// Version 5.0
// ----------------------------------------------------------
BinaryOpNode* FGRParser::expop()
{
	FGRToken tok = next_token();
	switch (tok.type)
	{
	case FGRTT_ROOT:
		match(FGRTT_ROOT);
		return new RootingNode(scanner.getLineNo());
		break;
	case FGRTT_EXP:
		match(FGRTT_EXP);
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
// Version 5.0
// ----------------------------------------------------------
void FGRParser::match(fgr_token_type toMatch)
{
	FGRToken tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = lookahead[1];
		lookahead[1] = FGRToken::NOTOK;
	}
	else
	{
		string type;
		switch (toMatch)
		{
		case FGRTT_ASSIGN:
			type = "=";
			break;
		case FGRTT_ADD:
			type = "++";
			break;
		case FGRTT_SUB:
			type = "--";
			break;
		case FGRTT_MUL:
			type = "**";
			break;
		case FGRTT_DIV:
			type = "//";
			break;
		case FGRTT_MOD:
			type = "%%";
			break;
		case FGRTT_IDIV:
			type = "\\\\";
			break;
		case FGRTT_ROOT:
			type = "##";
			break;
		case FGRTT_EXP:
			type = "^^";
			break;
		case FGRTT_NOT:
			type = "!";
			break;
		case FGRTT_LT: 
			type = "<";
			break;
		case FGRTT_GT:
			type = ">";
			break;
		case FGRTT_EQ:
			type = "==";
			break;
		case FGRTT_LTE:
			type = "<=";
			break;
		case FGRTT_GTE:
			type = ">=";
			break;
		case FGRTT_IF:
			type = "if";
			break;
		case FGRTT_THEN:
			type = "then";
			break;
		case FGRTT_ELSE:
			type = "else";
			break;
		case FGRTT_COLON:
			type = ":";
			break;
		case FGRTT_SEMICOLON:
			type = ";";
			break;
		case FGRTT_LPAREN:
			type = "(";
			break;
		case FGRTT_RPAREN:
			type = ")";
			break;
		case FGRTT_SCANEOF:
			type = "<EOF>";
			break;
		case FGRTT_STRING:
			type = "String Literal";
			break;
		case FGRTT_ID:
			type = "Identifier";
			break;
		case FGRTT_DOUBLECONST:
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
// Version 4.4
// ----------------------------------------------------------
void FGRParser::syntax_error(string err_msg)
{
	int lineNo = scanner.getLineNo();
	cout << "SYNTAX ERROR in file " << currFileName << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRParser::next_token()
{
	if (lookahead[0].type == FGRTT_NOTOK)
		lookahead[0] = scanner.scan();

	return lookahead[0];
}

// ----------------------------------------------------------
// This function populates and returns the second lookahead 
// token.
//
// Version 5.0
// ----------------------------------------------------------
FGRToken FGRParser::second_token()
{
	if (lookahead[1].type == FGRTT_NOTOK)
	{
		lookahead[0] = next_token();
		lookahead[1] = scanner.scan();
	}

	return lookahead[1];
}