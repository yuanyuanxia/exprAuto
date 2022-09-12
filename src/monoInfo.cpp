#include "monoInfo.hpp"
#include "funcInfo.hpp"

// TODO: poly&poly
// TODO: !!! Extract common factors, which can be done more finely. This is the basis for completing the simplification of (exp(x)-1)*log(x)*x
void monoInfo::combine(const struct monoInfo tmp)
{
    if (this->hasCommonFunc(tmp))
    {
        this->coefficient += tmp.coefficient;
        return;
    }
    else if (poly.monos.size() > 0)
    {
        monoInfo m1;
        m1.variables = {};
        m1.functions = tmp.functions;
        m1.coefficient = tmp.coefficient;
        poly.monos.push_back(m1);
        return;
    }
    else
    {
        monoInfo m1, m2;
        m1.variables = {};
        m1.functions = functions;
        m1.coefficient = coefficient;
        m2.variables = {};
        m2.functions = tmp.functions;
        m2.coefficient = tmp.coefficient;
        poly.monos.push_back(m1);
        poly.monos.push_back(m2);
        std::vector <funcInfo>().swap(functions); 
        // functions.swap(std::vector<funcInfo>()); // another way but error
        coefficient = 1;
        return;
    }
    fprintf(stderr, "monoInfo::combine(): please do something about functions!\n");
    return;
}

bool monoInfo::hasCommonCoef(const struct monoInfo &mono) const
{
    return (coefficient == mono.coefficient);
}

bool monoInfo::hasCommonFunc(const struct monoInfo &mono) const
{
    const std::vector<funcInfo> &funcs1 = functions;
    const std::vector<funcInfo> &funcs2 = mono.functions;
    size_t size1 = funcs1.size();
    size_t size2 = funcs2.size();
    if (size1 != size2)
    {
        return false;
    }

    for (size_t i = 0; i < size1; i++)
    {
        if (funcs1.at(i) != funcs2.at(i))
        {
            return false;
        }
    }
    return true;
}

bool monoInfo::hasCommonTerm(const struct monoInfo &mono) const
{
    const std::vector<variableInfo> &vars1 = variables;
    const std::vector<variableInfo> &vars2 = mono.variables;
    size_t size1 = vars1.size();
    size_t size2 = vars2.size();
    if (size1 != size2)
    {
        return false;
    }

    for (size_t i = 0; i < size1; i++)
    {
        if (vars1.at(i) != vars2.at(i))
        {
            return false;
        }
    }
    return true;
}

void monoInfo::showInfo()
{
    if(poly.monos.size() > 0) {
        for(size_t i = 0; i < poly.monos.size(); i++)
        {
            fprintf(stderr, "\t\"special\" coefficient %lu:\n", i);
            (poly.monos.at(i)).showInfo();
            fprintf(stderr, "\n");
        }
    }
    else
    {
        fprintf(stderr, "\tcoefficient = %f;\n", coefficient);
        for (size_t i = 0; i < functions.size(); i++)
        {
            (functions.at(i)).showInfo();
        }
    }
    // std::cout << "\tvariables size = " << variables.size() << std::endl;
    fprintf(stderr, "\tvariables size = %lu\n", variables.size());
    for (size_t i = 0; i < variables.size(); i++)
    {
        (variables.at(i)).showInfo();
    }
}

bool monoInfo::operator<(const monoInfo &mono) const
{
    const std::vector<variableInfo> &vars1 = variables;
    const std::vector<variableInfo> &vars2 = mono.variables;
    size_t size1 = vars1.size();
    size_t size2 = vars2.size();
    size_t size = std::min(size1, size2);

    for (size_t i = 0; i < size; i++)
    {
        if (vars1.at(i) < vars2.at(i))
        {
            return true;
        }
    }
    if (size1 < size2)
    {
        return true;
    }
    return false;
}

bool monoInfo::operator==(const monoInfo &mono) const
{
    return (this->hasCommonCoef(mono) && this->hasCommonFunc(mono) && this->hasCommonTerm(mono));
}

bool monoInfo::operator!=(const monoInfo &mono) const
{
    return !(*this == mono);
}
