//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.1
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "token.h"
using namespace std;

//static "constants" used for simplified processing of language
//static tokens
Token Token::NOTOK			 = Token(TT_NOTOK, "<None>");
Token Token::IF				 = Token(TT_IF, "if");
Token Token::THEN			 = Token(TT_THEN, "then");
Token Token::ELSE			 = Token(TT_ELSE, "else");
Token Token::LPAREN			 = Token(TT_LPAREN, "(");
Token Token::RPAREN			 = Token(TT_RPAREN, ")");
Token Token::EQUAL_SIGN		 = Token(TT_EQUAL_SIGN, "=");
Token Token::COLON			 = Token(TT_COLON, ":");
Token Token::SEMICOLON		 = Token(TT_SEMICOLON, ";");
Token Token::COMMA			 = Token(TT_COMMA, ",");
Token Token::OCTOTHORPE		 = Token(TT_OCTOTHORPE, "#");
Token Token::TILDE			 = Token(TT_TILDE, "~");
Token Token::DOT			 = Token(TT_DOT, ".");
Token Token::PERCENT		 = Token(TT_PERCENT, "%");
Token Token::DUAL_ADD		 = Token(TT_DUAL_ADD, "++");
Token Token::DUAL_SUB		 = Token(TT_DUAL_SUB, "--");
Token Token::DUAL_MUL		 = Token(TT_DUAL_MUL, "**");
Token Token::DUAL_DIV		 = Token(TT_DUAL_DIV, "//");
Token Token::DUAL_MOD		 = Token(TT_DUAL_MOD, "%%");
Token Token::DUAL_IDIV		 = Token(TT_DUAL_IDIV, "\\\\");
Token Token::DUAL_ROOT		 = Token(TT_DUAL_ROOT, "##");
Token Token::DUAL_EXP		 = Token(TT_DUAL_EXP, "^^");
Token Token::NOT			 = Token(TT_NOT, "!");
Token Token::LT				 = Token(TT_LT, "<");
Token Token::GT				 = Token(TT_GT, ">");
Token Token::DUAL_EQUAL_SIGN = Token(TT_DUAL_EQUAL_SIGN, "==");
Token Token::LTE			 = Token(TT_LTE, "<=");
Token Token::GTE			 = Token(TT_GTE, ">=");
Token Token::EOL			 = Token(TT_EOL, "<EOL>");
Token Token::SCANEOF		 = Token(TT_SCANEOF, "<EOF>");