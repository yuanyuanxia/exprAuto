#ifndef _MONOMIAL
#define _MONOMIAL
#include <iostream>
#include <vector>
#include "basic.hpp"
#include "funcInfo.hpp"
#include "variableInfo.hpp"
#include "polyInfo.hpp"

using std::vector;

struct funcInfo;

struct monoInfo
{
    double coefficient = 1;
    vector<funcInfo> functions;
    vector<variableInfo> variables;
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

monoInfo extractInfoKernel(const ast_ptr &expr);

vector<monoInfo> extractInfo(const vector<ast_ptr> &exprs);

vector<monoInfo> mergePolynomial(const vector<monoInfo> &info);

#endif
