#ifndef _GENEEXPR
#define _GENEEXPR

#include "basic.hpp"
#include "monoFracInfo.hpp"

using std::vector;

ast_ptr geneFunctionAST(const funcInfo &func);

ast_ptr geneMonomialAST(const monoInfo &monomial);

ast_ptr geneExprAST(const vector<monoInfo> &monos);

ast_ptr geneFunctionASTNew(const funcInfo &func);

ast_ptr geneMonomialAST(const monoFracInfo &monoFrac);

ast_ptr geneExprAST(const vector<monoFracInfo> &monoFracs);

ast_ptr geneExprASTYHR(vector<monoInfo> &info);


#endif