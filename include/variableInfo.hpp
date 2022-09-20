#ifndef _VARIABLEINFO
#define _VARIABLEINFO

struct variableInfo
{
    std::string name;
    int degree;

    void showInfo();

    bool operator<(const variableInfo &var) const;

    bool operator==(const variableInfo &var) const;

    bool operator!=(const variableInfo &var) const;
};

std::vector<variableInfo> mergeVariables(std::vector<variableInfo> vec1, std::vector<variableInfo> vec2);

#endif
