#ifndef _FUNCINFO
#define _FUNCINFO
#include <iostream>
#include <vector>
#include "monoInfo.hpp"
#include "polyInfo.hpp"

struct monoInfo;

struct funcInfo
{
    std::string callee;
    std::vector<polyInfo> args;

    void showInfo() const;

    bool operator==(const funcInfo &func) const;

    bool operator!=(const funcInfo &func) const;
};

#endif
