#ifndef _SIMPLIFYEXPR
#define _SIMPLIFYEXPR

#include "basic.hpp"

using std::string;

ast_ptr simplifyExpr(const string &exprStr);

ast_ptr simplifyExpr(const ast_ptr &expr);

void initPython();

void endPython();

ast_ptr simplifyExprFromStr(const string &exprStr);

#endif