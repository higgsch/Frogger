//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 5.0
// -----------------------------------------------------------------
// This program provides a compact interface for tokens.
// -----------------------------------------------------------------
#include "odfToken.h"
using namespace std;

//static "constants" used for simplified processing of language
//static tokens
ODFToken ODFToken::NOTOK		= ODFToken(ODFTT_NOTOK, "<None>");
ODFToken ODFToken::EQUALS		= ODFToken(ODFTT_EQUALS, "=");
ODFToken ODFToken::OCTOTHORPE	= ODFToken(ODFTT_OCTOTHORPE, "#");
ODFToken ODFToken::SEMICOLON	= ODFToken(ODFTT_SEMICOLON, ";");
ODFToken ODFToken::EOL			= ODFToken(ODFTT_EOL, "<End of Line>");
ODFToken ODFToken::SCANEOF		= ODFToken(ODFTT_SCANEOF, "<EOF>");