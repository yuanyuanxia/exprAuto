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

#endif
