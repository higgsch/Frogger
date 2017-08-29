//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
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
// Version 5.0
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
		string type;
		switch (toMatch)
		{
		case TT_EQUAL_SIGN:
			type = Token::EQUAL_SIGN.lexeme;
			break;
		case TT_DUAL_ADD:
			type = "++";
			break;
		case TT_DUAL_SUB:
			type = "--";
			break;
		case TT_DUAL_MUL:
			type = "**";
			break;
		case TT_DUAL_DIV:
			type = "//";
			break;
		case TT_DUAL_MOD:
			type = "%%";
			break;
		case TT_DUAL_IDIV:
			type = "\\\\";
			break;
		case TT_DUAL_ROOT:
			type = "##";
			break;
		case TT_DUAL_EXP:
			type = "^^";
			break;
		case TT_NOT:
			type = "!";
			break;
		case TT_LT: 
			type = "<";
			break;
		case TT_GT:
			type = ">";
			break;
		case TT_DUAL_EQUAL_SIGN:
			type = "==";
			break;
		case TT_LTE:
			type = "<=";
			break;
		case TT_GTE:
			type = ">=";
			break;
		case TT_IF:
			type = "if";
			break;
		case TT_THEN:
			type = "then";
			break;
		case TT_ELSE:
			type = "else";
			break;
		case TT_DOT:
			type = Token::DOT.lexeme;
			break;
		case TT_COMMA:
			type = Token::COMMA.lexeme;
			break;
		case TT_COLON:
			type = ":";
			break;
		case TT_SEMICOLON:
			type = Token::SEMICOLON.lexeme;
			break;
		case TT_LPAREN:
			type = Token::LPAREN.lexeme;
			break;
		case TT_RPAREN:
			type = Token::RPAREN.lexeme;
			break;
		case TT_TILDE:
			type = Token::TILDE.lexeme;
			break;
		case TT_OCTOTHORPE:
			type = Token::OCTOTHORPE.lexeme;
			break;
		case TT_EOL:
			type = Token::EOL.lexeme;
		case TT_SCANEOF:
			type = "<EOF>";
			break;
		case TT_STRINGCONST:
			type = "String Literal";
			break;
		case TT_ID:
			type = "Identifier";
			break;
		case TT_DOUBLECONST:
			type = "Double Literal";
			break;
		default:
			type = "<type>";
			break;
		}

		syn_error("", "Expected \'" + type + "\' - Found " + lookahead[0].lexeme);
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