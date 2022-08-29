#ifndef _CHANGEAST
#define _CHANGEAST

#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//

std::unique_ptr<ExprAST> expandExpr(const std::unique_ptr<ExprAST> &expr);

bool isEqual(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2);

std::unique_ptr<ExprAST> expandExprWrapper(const std::unique_ptr<ExprAST> &exprOrigin);

#endif
