#ifndef _SIMPLIFYEXPR
#define _SIMPLIFYEXPR

#include "basic.hpp"
std::unique_ptr<ExprAST> simplifyExpr(const std::string &exprStr);

std::unique_ptr<ExprAST> simplifyExpr(const std::unique_ptr<ExprAST> &expr);

void initPython();

void endPython();

#endif