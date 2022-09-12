#ifndef _PARSERASTLY
#define _PARSERASTLY

#include "basic.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> ParseExpressionForStr();

/// numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExprForStr();

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> ParseParenExprForStr();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> ParseIdentifierExprForStr();

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimaryForStr();

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ExprAST> ParseBinOpRHSForStr(int ExprPrec, std::unique_ptr<ExprAST> LHS);

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> ParseExpressionForStr();

std::string readFileIntoString(const char * filename);

std::unique_ptr<ExprAST> ParseExpressionFromString();

#endif