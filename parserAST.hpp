#ifndef _PARSERAST
#define _PARSERAST

#include "basic.hpp"
#include "laxerAST.hpp"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExpr();

/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> ParseParenExpr();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> ParseIdentifierExpr();

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimary();

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> ParseExpression();

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype();

/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition();

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> ParseTopLevelExpr();

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern();

#endif