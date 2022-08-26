#ifndef _FUNCINFO
#define _FUNCINFO
#include <iostream>
#include <vector>
#include "monoInfo.hpp"

struct monoInfo;

struct funcInfo
{
    std::string callee;
    std::vector<monoInfo> args;

    void showInfo();

    bool operator==(const funcInfo &func) const;

    bool operator!=(const funcInfo &func) const;
};

#endif
