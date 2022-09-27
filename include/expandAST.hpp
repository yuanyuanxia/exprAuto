#ifndef _CHANGEAST
#define _CHANGEAST

#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Change exprAST equally
//===----------------------------------------------------------------------===//

ast_ptr expandExpr(const ast_ptr &expr);

bool isEqual(const ast_ptr &expr1, const ast_ptr &expr2);

ast_ptr expandExprWrapper(const ast_ptr &exprOrigin);

#endif
