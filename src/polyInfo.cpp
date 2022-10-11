#include "monoFracInfo.hpp"
#include "polyInfo.hpp"

using std::vector;
using std::cout;
using std::endl;

void polyInfo::showInfo()
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "polyInfo::showInfo: ";
    cout << prompt << "start-----------" << endl;
    for(size_t i = 0; i < monoFracs.size(); ++i){
        // fprintf(stderr, "\t poly's the %luth monomial:\n", i);
        cout << prompt << "poly's the " << i << "th monomial:" << endl;
        // (monos.at(i)).showInfo();
        (monoFracs.at(i)).showInfo();
    }

    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return ;
}

bool polyInfo::operator==(const polyInfo &poly) const
{
    const auto &monoFracs1 = monoFracs;
    const auto &monoFracs2 = poly.monoFracs;
    size_t size1 = monoFracs1.size();
    size_t size2 = monoFracs2.size();
    if (size1 != size2)
    {
        return false;
    }

    for (size_t i = 0; i < size1; i++)
    {
        if (monoFracs1.at(i) != monoFracs2.at(i))
        {
            return false;
        }
    }
    return true;
}

bool polyInfo::operator!=(const polyInfo &poly) const
{
    return !(*this == poly);
}