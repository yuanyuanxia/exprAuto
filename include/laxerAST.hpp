#ifndef _LAXERAST
#define _LAXERAST

#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
    
    //StringExpr
    tok_string = -6
};

extern std::string IdentifierStr;  // Filled in if tok_identifier
extern double NumVal;              // Filled in if tok_number

/// gettok - Return the next token from standard input.
int gettok();

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
extern int CurTok;
int getNextToken();

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
extern std::map<char, int> BinopPrecedence;

// Install standard binary operators.
void installOperators();

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
int GetTokPrecedence();

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str);

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

#endif
