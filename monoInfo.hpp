#ifndef _MONOMIAL
#define _MONOMIAL
#include <iostream>
#include <vector>
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

    bool hasCommonCoef(const struct monoInfo mono) const;

    bool hasCommonFunc(const struct monoInfo mono) const;

    bool hasCommonTerm(const struct monoInfo mono) const;

    void showInfo();

    bool operator<(const monoInfo &mono) const;

    bool operator==(const monoInfo &mono) const;

    bool operator!=(const monoInfo &mono) const;
};

#endif
