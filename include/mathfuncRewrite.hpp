#ifndef _MATHFUNCTRANSAST
#define _MATHFUNCTRANSAST

#include "basic.hpp"

//===----------------------------------------------------------------------===//
// Equivalent transformation of mathematical function
//===----------------------------------------------------------------------===//

// exp(expr) - 1 ==> expm1(expr)
ast_ptr expToexpm1(const ast_ptr &expr);

// log(expr+1) ==> log1p(expr)
ast_ptr logTolog1p(const ast_ptr &expr);

// sqrt(expr1*expr1+expr2*expr2) ==> hypot(expr1, expr2)
ast_ptr sqrtTohypot(const ast_ptr &expr);

// log(exp(x))⇒x OR exp(log(x))⇒x
ast_ptr lex_x_Or_elx_x(const ast_ptr &expr);

// sqrt(x)*sqrt(y)⇒sqrt(x*y)
ast_ptr sqrtMult(const ast_ptr &expr);

// sqrt(x)/sqrt(y)⇒sqrt(x/y)
ast_ptr sqrtDiv(const ast_ptr &expr);

#endif
