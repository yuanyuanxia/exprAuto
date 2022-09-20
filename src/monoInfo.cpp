#include <algorithm>
#include "preprocess.hpp"
#include "monoInfo.hpp"

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

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2)
{
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    variableInfo variableTmp;

    coefficient = mono1.coefficient * mono2.coefficient;
    variables = mergeVariables(mono1.variables, mono2.variables);

    for(size_t i = 0; i < (mono1.functions).size(); ++i)
    {
        auto arg = (mono1.functions).at(i);
        functions.push_back(std::move(arg));
    }
    for(size_t i = 0; i < (mono2.functions).size(); ++i)
    {
        auto arg = (mono2.functions).at(i);
        functions.push_back(std::move(arg));
    }

    return monoFinal;
}

monoInfo extractInfoKernel(const std::unique_ptr<ExprAST> &expr)
{
    // fprintf(stderr, "extractInfoKernel: at the begin: expr = %s\n", PrintExpression(expr).c_str());
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    funcInfo funcTmp;
    variableInfo variableTmp;

    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if (exprType == "Number")
    {
        // fprintf(stderr, "extractInfoKernel: number\n");
        NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
        coefficient = numberPtr->getNumber();
        return monoFinal;
    }
    else if (exprType == "Variable")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        variableTmp.name = variablePtr->getVariable();
        variableTmp.degree = 1;
        variables.push_back(variableTmp);
        return monoFinal;
    }
    else if (exprType == "Call")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        funcTmp.callee = callPtr->getCallee();
        for(size_t i = 0; i < (callPtr->getArgs()).size(); ++i)
        {
            auto argAST = (callPtr->getArgs().at(i))->Clone();
            auto exprsTmp = extractItems(argAST);
            auto monosTmp = extractInfo(exprsTmp);
            polyInfo poly;
            for(auto monoTmp : monosTmp)
            {
                (poly.monos).push_back(monoTmp);
            }
            (funcTmp.args).push_back(poly);
        }
        functions.push_back(funcTmp);
        return monoFinal;
    }
    if (exprType != "Binary")
    {   // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractInfoKernel: expr is neither number, variable, call or binary\n");
        return monoFinal;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    if (op == '*')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '*'\n");
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '%c'\n", op);
    }
    return monoFinal;
}

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    // fprintf(stderr, "extractInfo: start--------\n");
    std::vector<monoInfo> results;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        monoInfo monoTmp = extractInfoKernel(exprTmp);
        if(monoTmp.coefficient != 0)
        {
            std::vector<variableInfo> &variables = monoTmp.variables;
            std::sort(variables.begin(), variables.end());
            results.push_back(monoTmp);
        }
    }

    // print information of info
    // for (long unsigned int i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The monoInfo No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    // fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

std::vector<monoInfo> mergePolynomial(const std::vector<monoInfo> &info)
{
    // fprintf(stderr, "mergePolynomial: start--------\n");
    size_t size = info.size();
    bool *merged = new bool[size]{};
    size_t i = 0, j = 0;
    std::vector<monoInfo> results;
    while (i < info.size())
    {
        if(merged[i])
        {
            i++;
            continue;
        }
        monoInfo tmp = info.at(i);
        merged[i] = true;
        j = i + 1;
        while (j < info.size())
        {
            monoInfo tmp1 = info.at(j);
            if(tmp.hasCommonTerm(tmp1))
            {
                tmp.combine(tmp1);
                merged[j] = true;
            }
            j++;
        }
        results.push_back(tmp);
    }
    // TODO: check sort. Eg: b*d + a*b + b*b + a*a, output should be a*a + a*b + b*b + b*d, but is a*a + b*b + a*b + b*d;
    std::sort(results.begin(), results.end());

    // print information of info
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The Monomial No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    delete []merged;
    // fprintf(stderr, "mergePolynomial: end----------\n");
    return results;
}
