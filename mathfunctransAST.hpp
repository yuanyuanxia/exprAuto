#ifndef _MATHFUNCTRANSAST
#define _MATHFUNCTRANSAST

#include "basic.hpp"
#include "changeAST.hpp"

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

// exp(expr) - 1 ==> expm1(expr)
std::unique_ptr<ExprAST> expToexpm1(const std::unique_ptr<ExprAST> &expr);

// log(expr+1) ==> log1p(expr)
std::unique_ptr<ExprAST> logTolog1p(const std::unique_ptr<ExprAST> &expr);

// sqrt(expr1*expr1+expr2*expr2) ==> hypot(expr1, expr2)
std::unique_ptr<ExprAST> sqrtTohypot(const std::unique_ptr<ExprAST> &expr);

// log(exp(x))⇒x OR exp(log(x))⇒x
std::unique_ptr<ExprAST> lex_x_Or_elx_x(std::unique_ptr<ExprAST> &expr);

#endif
