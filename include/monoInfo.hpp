#ifndef _MONOMIAL
#define _MONOMIAL
#include <iostream>
#include <vector>
#include "basic.hpp"
#include "funcInfo.hpp"
#include "variableInfo.hpp"
#include "polyInfo.hpp"

struct funcInfo;

struct monoInfo
{
    double coefficient = 1;
    std::vector<funcInfo> functions;
    std::vector<variableInfo> variables;
    polyInfo poly;

    void combine(const struct monoInfo tmp);

    bool hasCommonCoef(const struct monoInfo &mono) const;

    bool hasCommonFunc(const struct monoInfo &mono) const;

    bool hasCommonTerm(const struct monoInfo &mono) const;

    void showInfo();

    bool operator<(const monoInfo &mono) const;

    bool operator==(const monoInfo &mono) const;

    bool operator!=(const monoInfo &mono) const;
};

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2);

monoInfo extractInfoKernel(const std::unique_ptr<ExprAST> &expr);

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs);

std::vector<monoInfo> mergePolynomial(const std::vector<monoInfo> &info);

#endif
