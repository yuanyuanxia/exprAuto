#ifndef _LAXERASTLY
#define _LAXERASTLY

#include "basic.hpp"
#include "string.h"



enum Token1
{
    tok_eof_forstr = -1,

    // commands
    tok_def_forstr = -2,
    tok_extern_forstr = -3,

    // primary
    tok_identifier_forstr = -4,
    tok_number_forstr = -5
};
extern std::string filestring;
extern size_t flag;
extern int CurTokForStr;
extern std::string IdentifierStr1;  // Filled in if tok_identifier
extern double NumVal1;              // Filled in if tok_number

/// gettok - Return the next token from standard input.
int gettokForStr();

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.

int getNextTokenForStr();

/// BinopPrecedenceForStr - This holds the precedence for each binary operator that is
/// defined.
extern std::map<char, int> BinopPrecedenceForStr;

// Install standard binary operators.
void installOperatorsForStr();

/// GetTokPrecedenceForStr - Get the precedence of the pending binary operator token.
int GetTokPrecedenceForStr();

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogErrorForStr(const char *Str);

std::unique_ptr<PrototypeAST> LogErrorPForStr(const char *Str);

#endif