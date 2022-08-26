#include <iostream>
#include <vector>

#include "variableInfo.hpp"

void variableInfo::showInfo() { fprintf(stderr, "\tvariableInfo: name = %s; degree = %d\n", name.c_str(), degree); }

bool variableInfo::operator<(const variableInfo &var) const
{
    int result = name.compare(var.name); // compare: <0 means <; 0 means =; >0 means >
    if (result == 0)
    {
        return (degree < var.degree);
    }
    return (result < 0) ? true : false;
}

bool variableInfo::operator==(const variableInfo &var) const
{
    return ((name == var.name) && (degree == var.degree));
}

bool variableInfo::operator!=(const variableInfo &var) const
{
    return ((name != var.name) || (degree != var.degree));
}
