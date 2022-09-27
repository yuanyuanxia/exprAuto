#include <iostream>
#include <vector>

#include "variableInfo.hpp"

using std::vector;

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

vector<variableInfo> mergeVariables(vector<variableInfo> vec1, vector<variableInfo> vec2)
{
    vector<variableInfo> vec3;
    for (long unsigned int i = 0; i < vec1.size(); i++)
    {
        variableInfo &varTmp1 = vec1.at(i);
        variableInfo varTmp3 = varTmp1;

        for (long unsigned int j = 0; j < vec2.size(); j++)
        {
            variableInfo &varTmp2 = vec2.at(j);
            if (varTmp1.name == varTmp2.name)
            {
                varTmp3.degree = varTmp1.degree + varTmp2.degree;
                vec2.erase(vec2.begin() + j);
                break;
            }
        }
        vec3.push_back(varTmp3);
    }
    vec3.insert(vec3.end(), vec2.begin(), vec2.end());

    return vec3;
}
