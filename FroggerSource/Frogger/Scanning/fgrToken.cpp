//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "fgrToken.h"
using namespace std;

//static "constants" used for simplified processing of language
//static tokens
FGRToken FGRToken::NOTOK		= FGRToken(FGRTT_NOTOK, "<None>");
FGRToken FGRToken::ASSIGN		= FGRToken(FGRTT_ASSIGN, "=");
FGRToken FGRToken::ADD			= FGRToken(FGRTT_ADD, "++");
FGRToken FGRToken::SUB			= FGRToken(FGRTT_SUB, "--");
FGRToken FGRToken::MUL			= FGRToken(FGRTT_MUL, "**");
FGRToken FGRToken::DIV			= FGRToken(FGRTT_DIV, "//");
FGRToken FGRToken::MOD			= FGRToken(FGRTT_MOD, "%%");
FGRToken FGRToken::IDIV			= FGRToken(FGRTT_IDIV, "\\\\");
FGRToken FGRToken::ROOT			= FGRToken(FGRTT_ROOT, "##");
FGRToken FGRToken::EXP			= FGRToken(FGRTT_EXP, "^^");
FGRToken FGRToken::NOT			= FGRToken(FGRTT_NOT, "!");
FGRToken FGRToken::LT			= FGRToken(FGRTT_LT, "<");
FGRToken FGRToken::GT			= FGRToken(FGRTT_GT, ">");
FGRToken FGRToken::EQ			= FGRToken(FGRTT_EQ, "==");
FGRToken FGRToken::LTE			= FGRToken(FGRTT_LTE, "<=");
FGRToken FGRToken::GTE			= FGRToken(FGRTT_GTE, ">=");
FGRToken FGRToken::IF			= FGRToken(FGRTT_IF, "if");
FGRToken FGRToken::THEN			= FGRToken(FGRTT_THEN, "then");
FGRToken FGRToken::ELSE			= FGRToken(FGRTT_ELSE, "else");
FGRToken FGRToken::COMMA		= FGRToken(FGRTT_COMMA, ",");
FGRToken FGRToken::COLON		= FGRToken(FGRTT_COLON, ":");
FGRToken FGRToken::SEMICOLON	= FGRToken(FGRTT_SEMICOLON, ";");
FGRToken FGRToken::LPAREN		= FGRToken(FGRTT_LPAREN, "(");
FGRToken FGRToken::RPAREN		= FGRToken(FGRTT_RPAREN, ")");
FGRToken FGRToken::SCANEOF		= FGRToken(FGRTT_SCANEOF, "<EOF>");