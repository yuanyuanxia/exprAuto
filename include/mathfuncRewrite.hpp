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

// x*y+a, y*x+a, a+x*y, a+y*x => fma(x,y,a), x*y+a*b, y*x+a*b, x*y+b*a, y*x+a*b,  => fma(x,y,a*b) or fma(a,b,x*y)
vector<ast_ptr> checkFma(vector<ast_ptr> &exprs);

vector<ast_ptr> toFma(const ast_ptr &expr);

vector<ast_ptr> fmaRewriteKernel(vector<ast_ptr> &exprs, const char &op);

vector<ast_ptr> fmaRewrite(const ast_ptr &expr);

ast_ptr toPow(const ast_ptr &expr);

ast_ptr sumToProduct(const ast_ptr &expr);

ast_ptr fmaToMulAndAdd(const ast_ptr &expr);

vector<ast_ptr> powCombine(const ast_ptr& expr);

vector<ast_ptr> sqrtCombine(const ast_ptr& expr);

#endif
