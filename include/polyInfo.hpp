#ifndef _POLYINFO
#define _POLYINFO
#include <iostream>
#include <vector>
#include "funcInfo.hpp"
#include "variableInfo.hpp"
#include "monoFracInfo.hpp"

struct monoInfo;
struct monoFracInfo;

struct polyInfo
{
    std::vector<monoInfo> monos;
    std::vector<monoFracInfo> monoFracs;

    void showInfo();

    bool operator<(const polyInfo &poly) const;

    bool operator==(const polyInfo &poly) const;

    bool operator!=(const polyInfo &poly) const;
};

#endif
