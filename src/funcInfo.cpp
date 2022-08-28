#include "funcInfo.hpp"
#include "monoInfo.hpp"

void funcInfo::showInfo()
{
    std::cout << "\tfuncInfo: callee = " << callee.c_str() << ", args size = " << args.size() << std::endl;
    for(size_t i = 0; i < args.size(); i++)
    {
        (args.at(i)).showInfo();
    }
}

bool funcInfo::operator==(const funcInfo &func) const
{
    if (callee != func.callee)
    {
        return false;
    }
    size_t size1 = args.size();
    size_t size2 = func.args.size();
    if (size1 != size2)
    {
        return false;
    }

    for (size_t i = 0; i < size1; i++)
    {
        if (args.at(i) != (func.args).at(i))
        {
            return false;
        }
    }
    return true;
}

bool funcInfo::operator!=(const funcInfo &func) const
{
    return !(*this == func);
}
