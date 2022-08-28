#ifndef _POLYINFO
#define _POLYINFO
#include <iostream>
#include <vector>
#include "funcInfo.hpp"
#include "variableInfo.hpp"
#include "monoInfo.hpp"

struct monoInfo;

struct polyInfo
{
    std::vector<monoInfo> monos;

    void showInfo();

    bool operator<(const polyInfo &poly) const;

    bool operator==(const polyInfo &poly) const;

    bool operator!=(const polyInfo &poly) const;
};

#endif
