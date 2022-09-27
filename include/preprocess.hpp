#ifndef _PREPROCESS
#define _PREPROCESS

#include "basic.hpp"

using std::vector;

ast_ptr combineFraction(const ast_ptr &numerator, const ast_ptr &denominator);

vector<ast_ptr> extractItems(const ast_ptr &expr);

ast_ptr moveDivKernel(const ast_ptr &expr);

vector<ast_ptr> moveDiv(const vector<ast_ptr> &exprs);

ast_ptr mergeFraction(const vector<ast_ptr> &exprs);

ast_ptr minusRewrite(const ast_ptr &expr);

ast_ptr preprocessInit(const ast_ptr &expr);

ast_ptr preprocess(const ast_ptr &expr);

#endif
