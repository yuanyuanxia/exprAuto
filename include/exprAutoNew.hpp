#ifndef _EXPRAUTONEW
#define _EXPRAUTONEW

#include <iostream>
#include <string>
#include <vector>
#include "basic.hpp"
#include "preprocess.hpp"
// #include "exprAuto.hpp"

// similiar to getExprFromVariants
ast_ptr getCallFromVariants(const std::vector<ast_ptr> &variants, const std::vector<size_t> orders, const std::string callee);

// similiar to combineMonomial
std::vector<ast_ptr> combineCallArgs(const std::vector<ast_ptr> &variants, const std::vector<size_t> widths, const std::string callee);

ast_ptr dealWithCallsKernel(const ast_ptr &expr, const std::string callee);

ast_ptr dealWithCallsKernel(const ast_ptr &expr);

std::vector<std::unique_ptr<ExprAST>> createAllBinary(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators, const char op);

std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr, const char &op);

std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> dealWithBinOp(std::vector<std::unique_ptr<ExprAST>> &exprs, const char &op);

// TODO: delete the same expr in exprs using the function 'isEqual'
void deleteTheSame(std::vector<ast_ptr> &exprs);

std::vector<ast_ptr> dealWithCalls(const ast_ptr &expr);

std::vector<ast_ptr> mathfuncRewrite(const ast_ptr &expr);

std::vector<size_t> getCombineOrders(const std::vector<size_t> widths, const size_t identifier);

std::vector<ast_ptr> tryRewrite(ast_ptr expr);

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators);

std::vector<ast_ptr> exprAutoNew(const ast_ptr &expr);

#endif
