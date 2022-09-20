#ifndef _PREPROCESS
#define _PREPROCESS

#include "basic.hpp"

std::unique_ptr<ExprAST> combineFraction(const std::unique_ptr<ExprAST> &numerator, const std::unique_ptr<ExprAST> &denominator);

std::vector<std::unique_ptr<ExprAST>> extractItems(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> moveDivKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<std::unique_ptr<ExprAST>> moveDiv(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::unique_ptr<ExprAST> mergeFraction(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::unique_ptr<ExprAST> minusRewrite(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> preprocessInit(const std::unique_ptr<ExprAST> &expr);

std::unique_ptr<ExprAST> preprocess(const std::unique_ptr<ExprAST> &expr);

#endif
