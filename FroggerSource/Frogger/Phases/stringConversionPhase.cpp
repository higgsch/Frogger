//                      Christopher Higgs
//                      FROGGER Compiler
//                      Version: 3.0
// -----------------------------------------------------------------
// This program represents a visitor for converting string literals
// from frogger syntax to c++ syntax.
// -----------------------------------------------------------------
#include "stringConversionPhase.h"
using namespace std;

// ----------------------------------------------------------
// This function converts the given frogger string to c++
// syntax.
// @s: The string to convert.
//
// Version 1.0
// ----------------------------------------------------------
string StringConversionPhase::convertString(string s)
{
	string result = "\"";
	for (unsigned int i = 1; i < s.length() - 1; i++) //First and final char is "\'"
	{
		int c = s[i];
		if (c == '\\') // 'a\nb' is not an escaped newline but a literal slash
			result += "\\\\";
		else if (c != '&')
			result += c;
		else
		{
			c = s[++i];
			if (c != '&') //&& is the only non-escaped translation to c++
				result += "\\";
			result += c;
		}
	}
	result += "\"";
	return result;
}

// ----------------------------------------------------------
// This function processes a string literal.
// @n: The node representing the string.
//
// Version 1.0
// ----------------------------------------------------------
void StringConversionPhase::visit(StringConstingNode * n)
{
	n->setLexeme(convertString(n->getLexeme()));
}