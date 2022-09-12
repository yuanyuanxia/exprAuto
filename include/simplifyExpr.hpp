#ifndef _SIMPLIFYEXPR
#define _SIMPLIFYEXPR

#include "basic.hpp"
std::unique_ptr<ExprAST> simplifyExpr(std::unique_ptr<ExprAST> &expr);

#endif