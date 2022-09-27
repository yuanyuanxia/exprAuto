#ifndef _GENEEXPR
#define _GENEEXPR

#include "basic.hpp"
#include "monoInfo.hpp"

std::unique_ptr<ExprAST> geneFunctionAST(const funcInfo &func);

std::unique_ptr<ExprAST> geneMonomialAST(const monoInfo &monomial);

std::unique_ptr<ExprAST> geneExprAST(const std::vector<monoInfo> &monos);

std::unique_ptr<ExprAST> geneExprASTYHR(std::vector<monoInfo> &info);


#endif