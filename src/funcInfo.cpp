#include "monoFracInfo.hpp"
#include "funcInfo.hpp"

using std::cout;
using std::endl;

void funcInfo::showInfo() const
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "funcInfo::showInfo: ";
    cout << prompt << "start-----------" << endl;

    // std::cout << "\tfuncInfo: callee = " << callee.c_str() << ", args size = " << args.size() << std::endl;
    cout << prompt << "callee = " << callee << ", args size = " << args.size() << endl;
    for(size_t i = 0; i < args.size(); i++)
    {
        (args.at(i)).showInfo();
    }

    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return ;
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
