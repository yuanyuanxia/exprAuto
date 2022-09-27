#include "monoInfo.hpp"
#include "polyInfo.hpp"

using std::vector;

void polyInfo::showInfo()
{
    for(size_t i = 0; i < monos.size(); ++i){
        fprintf(stderr, "\t poly's the %luth monomial:\n", i);
        (monos.at(i)).showInfo();
    }
}

bool polyInfo::operator==(const polyInfo &poly) const
{
    const vector<monoInfo> &monos1 = monos;
    const vector<monoInfo> &monos2 = poly.monos;
    size_t size1 = monos1.size();
    size_t size2 = monos2.size();
    if (size1 != size2)
    {
        return false;
    }

    for (size_t i = 0; i < size1; i++)
    {
        if (monos1.at(i) != monos2.at(i))
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