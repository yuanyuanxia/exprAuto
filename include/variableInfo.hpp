#ifndef _VARIABLEINFO
#define _VARIABLEINFO

using std::vector;

struct variableInfo
{
    std::string name;
    int degree;

    void showInfo();

    bool operator<(const variableInfo &var) const;

    bool operator==(const variableInfo &var) const;

    bool operator!=(const variableInfo &var) const;
};

vector<variableInfo> mergeVariables(vector<variableInfo> vec1, vector<variableInfo> vec2);

#endif
