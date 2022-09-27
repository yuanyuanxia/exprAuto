#ifndef _PARSERAST
#define _PARSERAST

#include "basic.hpp"
#include "laxerAST.hpp"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

ast_ptr ParseExpression();

/// numberexpr ::= number
ast_ptr ParseNumberExpr();

/// parenexpr ::= '(' expression ')'
ast_ptr ParseParenExpr();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
ast_ptr ParseIdentifierExpr();

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
ast_ptr ParsePrimary();

/// binoprhs
///   ::= ('+' primary)*
ast_ptr ParseBinOpRHS(int ExprPrec, ast_ptr LHS);

/// expression
///   ::= primary binoprhs
///
ast_ptr ParseExpression();

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