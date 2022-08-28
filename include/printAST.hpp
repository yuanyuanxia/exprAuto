#ifndef _PRINTAST
#define _PRINTAST
#include "basic.hpp"

//===----------------------------------------------------------------------===//
// print information
//===----------------------------------------------------------------------===//

std::string PrintExpression(std::unique_ptr<ExprAST> &expr);

void PrintFunction(std::unique_ptr<FunctionAST> &fun);

#endif
