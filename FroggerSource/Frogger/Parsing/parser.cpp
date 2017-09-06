//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.1
// -----------------------------------------------------------------
// This program provides the functionality to interpret a file
// -----------------------------------------------------------------
#include "parser.h"
using namespace std;

// ----------------------------------------------------------
// This is the primary constructor.
// @scanner: The associated scanner.
//
// Version 5.0
// ----------------------------------------------------------
Parser::Parser(Scanner * scanner) : scanner(scanner), currFilePath(""), 
	current_token(Token::NOTOK) 
{
	lookahead[0] = Token::NOTOK;
	lookahead[1] = Token::NOTOK;
}

// ----------------------------------------------------------
// This function opens the SCF and checks for existance.
// @SCFPath: The path to the SCF.
//
// Version 5.0
// ----------------------------------------------------------
void Parser::open(string path)
{
	scanner->open(path);

	if (!scanner->good())
	{
		scanner->close();
		syn_error("", "File not found: " + path);
	}

	currFilePath = path;
}

// ----------------------------------------------------------
// This function processes and returns a function name.
//
// Version 5.0
// ----------------------------------------------------------
string Parser::id()
{
	Token id = next_token();
	match(TT_ID);

	return id.lexeme;
}

// ----------------------------------------------------------
// This function tests if the next token matches toMatch and
// moves to the next token on success. It displayes an error
// on failure.
// @toMatch: The expected token category.
//
// Version 5.1
// ----------------------------------------------------------
void Parser::match(token_type toMatch)
{
	Token tok = next_token();
	if (tok.type == toMatch)
	{
		current_token = lookahead[0];
		lookahead[0] = lookahead[1];
		lookahead[1] = Token::NOTOK;
	}
	else
	{
		Token type;
		switch (toMatch)
		{
		case TT_EQUAL_SIGN:
			type = Token::EQUAL_SIGN;
			break;
		case TT_DUAL_ADD:
			type = Token::DUAL_ADD;
			break;
		case TT_DUAL_SUB:
			type = Token::DUAL_SUB;
			break;
		case TT_DUAL_MUL:
			type = Token::DUAL_MUL;
			break;
		case TT_DUAL_DIV:
			type = Token::DUAL_DIV;
			break;
		case TT_DUAL_MOD:
			type = Token::DUAL_MOD;
			break;
		case TT_DUAL_IDIV:
			type = Token::DUAL_IDIV;
			break;
		case TT_DUAL_ROOT:
			type = Token::DUAL_ROOT;
			break;
		case TT_DUAL_EXP:
			type = Token::DUAL_EXP;
			break;
		case TT_NOT:
			type = Token::NOT;
			break;
		case TT_LT: 
			type = Token::LT;
			break;
		case TT_GT:
			type = Token::GT;
			break;
		case TT_DUAL_EQUAL_SIGN:
			type = Token::DUAL_EQUAL_SIGN;
			break;
		case TT_LTE:
			type = Token::LTE;
			break;
		case TT_GTE:
			type = Token::GTE;
			break;
		case TT_IF:
			type = Token::IF;
			break;
		case TT_THEN:
			type = Token::THEN;
			break;
		case TT_ELSE:
			type = Token::ELSE;
			break;
		case TT_DOT:
			type = Token::DOT;
			break;
		case TT_PERCENT:
			type = Token::PERCENT;
			break;
		case TT_COMMA:
			type = Token::COMMA;
			break;
		case TT_COLON:
			type = Token::COLON;
			break;
		case TT_SEMICOLON:
			type = Token::SEMICOLON;
			break;
		case TT_LPAREN:
			type = Token::LPAREN;
			break;
		case TT_RPAREN:
			type = Token::RPAREN;
			break;
		case TT_TILDE:
			type = Token::TILDE;
			break;
		case TT_OCTOTHORPE:
			type = Token::OCTOTHORPE;
			break;
		case TT_EOL:
			type = Token::EOL;
		case TT_SCANEOF:
			type = Token::SCANEOF;
			break;
		case TT_STRINGCONST:
			type = Token(TT_STRINGCONST,"String Literal");
			break;
		case TT_ID:
			type = Token(TT_ID,"Identifier");
			break;
		case TT_DOUBLECONST:
			type = Token(TT_DOUBLECONST,"Double Literal");
			break;
		default:
			type = Token(TT_NOTOK,"<type>");
			break;
		}

		syn_error("", "Expected \'" + type.lexeme + "\' - Found " + lookahead[0].lexeme);
	}
}

// ----------------------------------------------------------
// This function populates and returns the lookahead token.
//
// Version 5.0
// ----------------------------------------------------------
Token Parser::next_token()
{
	if (lookahead[0].type == TT_NOTOK)
		lookahead[0] = scanner->scan();

	return lookahead[0];
}

// ----------------------------------------------------------
// This function populates and returns the second lookahead 
// token.
//
// Version 5.0
// ----------------------------------------------------------
Token Parser::second_token()
{
	if (lookahead[1].type == TT_NOTOK)
	{
		lookahead[0] = next_token();
		lookahead[1] = scanner->scan();
	}

	return lookahead[1];
}

extern bool quietMode;

// ----------------------------------------------------------
// This function displays an error to the user and terminates
// the program.
// @err_msg: The message to display.
//
// Version 5.0
// ----------------------------------------------------------
void Parser::syn_error(string type, string err_msg)
{
	int lineNo = scanner->getLineNo();
	cout << ((type == "") ? "" : type + " ") << "SYNTAX ERROR in file " << currFilePath << " on line " << lineNo << ": " << err_msg << endl;
	
	if (!quietMode)
	{
		cout << "Press Enter to Exit" << endl;

		getchar();
	}
	exit(0);
}