#ifndef _EXPRAUTO
#define _EXPRAUTO

#include "monoInfo.hpp"

bool isFraction(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> getNumerator(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> getDenominator(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> createBinaryExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2, const char op);

std::unique_ptr<ExprAST> addExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2);

std::unique_ptr<ExprAST> mulExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2);

std::unique_ptr<ExprAST> divExpr(const std::unique_ptr<ExprAST> &expr1, const std::unique_ptr<ExprAST> &expr2);

std::unique_ptr<ExprAST> combineFraction(const std::unique_ptr<ExprAST> &numerator, const std::unique_ptr<ExprAST> &denominator);

std::vector<std::unique_ptr<ExprAST>> extractItems(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> moveDivKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> moveDiv(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::unique_ptr<ExprAST> mergeFraction(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::unique_ptr<ExprAST> preprocessInit(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> preprocess(const std::unique_ptr<ExprAST> &expr);

std::vector<variableInfo> mergeVariables(std::vector<variableInfo> vec1, std::vector<variableInfo> vec2);

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2);

monoInfo extractInfoKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::vector<monoInfo> mergePolynomial(const std::vector<monoInfo> &info);

std::unique_ptr<ExprAST> geneExprAST(std::vector<monoInfo> &info);

// TODO: implement. rewriteExpr should contain poly and math function equal change
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::unique_ptr<ExprAST> &expr);

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators);

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST> &expr);

#endif
