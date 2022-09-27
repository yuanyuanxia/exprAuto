#ifndef _EXPRAUTO
#define _EXPRAUTO

#include "monoInfo.hpp"

std::unique_ptr<ExprAST> dealWithCallsKernel(const std::unique_ptr<ExprAST> &expr, const std::string callee);

std::unique_ptr<ExprAST> dealWithCallsKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> createAllBinary(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators, const char op);

std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr, const char &op);

std::unique_ptr<ExprAST> dealWithBinOpKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> dealWithBinOp(std::vector<std::unique_ptr<ExprAST>> &exprs, const char &op);

std::vector<std::unique_ptr<ExprAST>> mathfuncRewriteNew(const std::unique_ptr<ExprAST> &expr);

std::vector<size_t> getCombineOrders(const std::vector<size_t> widths, const size_t identifier);

// TODO: implement. rewriteExpr should contain poly and math function equal change
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::unique_ptr<ExprAST> &expr);

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators);

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST> &expr);

#endif
