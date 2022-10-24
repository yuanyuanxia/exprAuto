#ifndef _EXPRAUTONEW
#define _EXPRAUTONEW

#include <iostream>
#include <string>
#include <vector>
#include "basic.hpp"
#include "preprocess.hpp"

using std::string;
using std::vector;

// conbine the constants in expr
void combineConstant(ast_ptr &expr);

// similiar to getExprFromVariants
ast_ptr getCallFromVariants(const vector<ast_ptr> &variants, const vector<size_t> orders, const string callee);

// similiar to combineMonomial
vector<ast_ptr> combineCallArgs(const vector<ast_ptr> &variants, const vector<size_t> widths, const string callee);

ast_ptr dealWithCallsKernel(const ast_ptr &expr, const string callee);

ast_ptr dealWithCallsKernel(const ast_ptr &expr);

vector<ast_ptr> createAllBinary(vector<ast_ptr> &numerators, vector<ast_ptr> &denominators, const char op);

ast_ptr dealWithBinOpKernel(const ast_ptr &expr, const char &op);

ast_ptr dealWithBinOpKernel(const ast_ptr &expr);

vector<ast_ptr> dealWithBinOp(vector<ast_ptr> &exprs, const char &op);

// TODO: delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(vector<ast_ptr> &exprs);

vector<ast_ptr> dealWithCalls(const ast_ptr &expr);

vector<ast_ptr> mathfuncRewrite(const ast_ptr &expr);

vector<size_t> getCombineOrders(const vector<size_t> widths, const size_t identifier);

void sortExpr(ast_ptr &expr);

vector<ast_ptr> tryRewrite(ast_ptr expr);

vector<ast_ptr> createAll(vector<ast_ptr> &numerators, vector<ast_ptr> &denominators);

vector<ast_ptr> exprAutoNew(const ast_ptr &expr);

#endif
