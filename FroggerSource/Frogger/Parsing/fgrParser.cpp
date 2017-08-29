//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program parses a stream of tokens to determine validity in 
// the frogger language and builds an AST for the input source code. 
// -----------------------------------------------------------------
#include "fgrParser.h"
using namespace std;

// ----------------------------------------------------------
// This function opens the input file.
// @inFile: The .fgr file to open.
// Note: File reference from project directory.
//
// Version 4.0
// ----------------------------------------------------------
void FGRParser::open(string inFile)
{
	currFilePath = inFile;
	scanner.openAndInitialize(inFile);
}

// ----------------------------------------------------------
// This function closes the input file stream.
//
// Version 4.0
// ----------------------------------------------------------
void FGRParser::close()
{
	currFilePath = "";
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
	Token tok = next_token();
	if (tok.type == TT_SCANEOF)
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
	Token tok = next_token();
	switch(tok.type)
	{
	case TT_SCANEOF:
		//lambda
		return NULL;
		break;
	case TT_IF:
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
	Token tok = next_token();
	switch(tok.type)
	{
	case TT_IF:
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

	Token tok = next_token();
	switch (tok.type)
	{
	case TT_IF:
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
	match(TT_IF); match(TT_LPAREN);
	AsciiNode* toCompare = boolexp();
	match(TT_RPAREN); match(TT_THEN);
	ControlFlowNode* trueStmt = nestedflowstmt();
	match(TT_ELSE);
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

	Token tok = next_token();

	NotingNode* not = NULL;
	if (tok.type == TT_NOT)
	{
		match(TT_NOT); 
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
	Token tok = next_token();
	switch (tok.type)
	{
	case TT_LT:
		match(TT_LT);
		return new LTingNode(scanner.getLineNo());
		break;
	case TT_GT:
		match(TT_GT);
		return new GTingNode(scanner.getLineNo());
		break;
	case TT_DUAL_EQUAL_SIGN:
		match(TT_DUAL_EQUAL_SIGN);
		return new EQingNode(scanner.getLineNo());
		break;
	case TT_LTE:
		match(TT_LTE);
		return new LTEingNode(scanner.getLineNo());
		break;
	case TT_GTE:
		match(TT_GTE);
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

	Token idTok = next_token();
	Token tok = second_token();

	switch (tok.type)
	{
	case TT_COLON:
		{
			AsciiNode * cmd = typedterm(false);
			match(TT_SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case TT_LPAREN:
		{
			match(TT_ID);
			match(TT_LPAREN);
			CommandCallNode* cmd = new CommandCallNode(DataType::DT_NULL, idTok.lexeme, scanner.getLineNo());

			Token argTok = next_token();
			if (argTok.type != TT_RPAREN)
			{
				cmd->addArgList(arglist(0, cmd->getCmd()));
			}

			match(TT_RPAREN);
			match(TT_SEMICOLON);

			stmt->setStmt(cmd);
			break;
		}
	case TT_EQUAL_SIGN:
		{
			match(TT_ID);
			IdRefNode* id = new IdRefNode(idTok.lexeme, scanner.getLineNo());
			match(TT_EQUAL_SIGN);

			AsciiNode* toAssign = expr();
			match(TT_SEMICOLON);

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

	if (next_token().type == TT_COMMA)
	{
		match(TT_COMMA);
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

	Token tok = next_token();
	switch(tok.type)
	{
	case TT_DUAL_ADD: //fall through
	case TT_DUAL_SUB:
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

	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DUAL_MUL: //fall through
	case TT_DUAL_DIV: //fall through
	case TT_DUAL_MOD: //fall through
	case TT_DUAL_IDIV:
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

	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DUAL_ROOT: //fall through
	case TT_DUAL_EXP: 
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
		Token tok = next_token();
		if (tok.type == TT_COLON)
		{
			match(TT_COLON);

			Token idTok = next_token();
			match(TT_ID);
			FunctionCallNode * funct = new FunctionCallNode(idTok.lexeme, scanner.getLineNo());
			match(TT_LPAREN);

			Token firstArg = next_token();

			if (firstArg.type != TT_RPAREN)
			{
				funct->addArgList(arglist(0,funct->getFunct()));
			}

			match(TT_RPAREN);

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
				cmd->setCmd(endRoutine->getFunct());
				cmd->getCmd()->returnType = DataType::DT_NULL;
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
	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DOUBLECONST:
		match(TT_DOUBLECONST);
		return new DoubleConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case TT_ID:
		{
			match(TT_ID);
		
			Token nextTok = next_token();
			if (nextTok.type != TT_LPAREN)
				return new IdRefNode(tok.lexeme, scanner.getLineNo());

			//Function call
			FunctionCallNode * funct = new FunctionCallNode(tok.lexeme, scanner.getLineNo());
			funct->getFunct()->primary = DataType::DT_NULL;
			match(TT_LPAREN);

			Token firstArg = next_token();
			if (firstArg.type != TT_RPAREN)
			{
				funct->addArgList(arglist(0, funct->getFunct()));
			}

			match(TT_RPAREN);
			return funct;
			break;
		}
	case TT_STRINGCONST:
		match(TT_STRINGCONST);
		return new StringConstingNode(tok.lexeme, scanner.getLineNo());
		break;
	case TT_LPAREN:
		{
			match(TT_LPAREN); 
			AsciiNode* val = expr(); 
			match(TT_RPAREN);
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
	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DUAL_ADD:
		match(tok.type);
		return new AddingNode(scanner.getLineNo());
		break;
	case TT_DUAL_SUB:
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
	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DUAL_MUL:
		match(TT_DUAL_MUL);
		return new MulingNode(scanner.getLineNo());
		break;
	case TT_DUAL_DIV:
		match(TT_DUAL_DIV);
		return new DivingNode(scanner.getLineNo());
		break;
	case TT_DUAL_MOD:
		match(TT_DUAL_MOD);
		return new ModDivingNode(scanner.getLineNo());
		break;
	case TT_DUAL_IDIV:
		match(TT_DUAL_IDIV);
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
	Token tok = next_token();
	switch (tok.type)
	{
	case TT_DUAL_ROOT:
		match(TT_DUAL_ROOT);
		return new RootingNode(scanner.getLineNo());
		break;
	case TT_DUAL_EXP:
		match(TT_DUAL_EXP);
		return new ExpingNode(scanner.getLineNo());
		break;
	default:
		syntax_error("Expected '##' or '^^' - Found " + tok.lexeme);
		return NULL;
		break;
	}
}