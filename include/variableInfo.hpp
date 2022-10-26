#ifndef _VARIABLEINFO
#define _VARIABLEINFO

#include "basic.hpp"

using std::vector;
using std::string;

struct variableInfo
{
    std::string name;
    int degree;

    void showInfo() const;

    bool operator<(const variableInfo &var) const;

    bool operator==(const variableInfo &var) const;

    bool operator!=(const variableInfo &var) const;

    string printStr() const;
};

vector<variableInfo> mergeVariables(vector<variableInfo> vec1, vector<variableInfo> vec2);

#endif
