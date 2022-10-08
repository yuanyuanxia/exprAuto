#ifndef _MONOFRACINFO
#define _MONOFRACINFO

#include "monoInfo.hpp"

struct monoInfo;

struct monoFracInfo
{
    double coefficient = 1;
    monoInfo numerator;
    monoInfo denominator;

    void combine(const struct monoFracInfo tmp);

    void showInfo();

    bool operator<(const monoFracInfo &fraction) const;

    bool operator==(const monoFracInfo &fraction) const;

    bool operator!=(const monoFracInfo &fraction) const;
};

monoFracInfo extractFracInfoKernel(const ast_ptr &expr);

vector<monoFracInfo> extractFracInfo(const vector<ast_ptr> &exprs);

bool checkMonoFrac(const vector<monoFracInfo> &infos);

#endif