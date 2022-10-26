#include <algorithm>
#include "preprocess.hpp"
#include "monoFracInfo.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

// TODO: poly&poly
// TODO: !!! Extract common factors, which can be done more finely. This is the basis for completing the simplification of (exp(x)-1)*log(x)*x
void monoInfo::combine(const struct monoInfo tmp)
{
    if (this->hasCommonFunc(tmp))
    {
        this->coefficient += tmp.coefficient;
        return;
    }
    else if (poly.monoFracs.size() > 0)
    {
        monoFracInfo m1;
        monoInfo &numerator = m1.numerator;
        numerator.variables = {};
        numerator.functions = tmp.functions;
        numerator.coefficient = tmp.coefficient;
        poly.monoFracs.push_back(m1);
        return;
    }
    else
    {
        monoFracInfo m1, m2;
        monoInfo &numerator1 = m1.numerator;
        monoInfo &numerator2 = m2.numerator;
        numerator1.variables = {};
        numerator1.functions = functions;
        numerator1.coefficient = coefficient;
        numerator2.variables = {};
        numerator2.functions = tmp.functions;
        numerator2.coefficient = tmp.coefficient;
        poly.monoFracs.push_back(m1);
        poly.monoFracs.push_back(m2);
        vector <funcInfo>().swap(functions); 
        // functions.swap(vector<funcInfo>()); // another way but error
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
    const vector<funcInfo> &funcs1 = functions;
    const vector<funcInfo> &funcs2 = mono.functions;
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
    const vector<variableInfo> &vars1 = variables;
    const vector<variableInfo> &vars2 = mono.variables;
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

void monoInfo::showInfo() const
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "monoInfo::showInfo: ";
    cout << prompt << "start-----------" << endl;

    if(poly.monoFracs.size() > 0) {
        for(size_t i = 0; i < poly.monoFracs.size(); i++)
        {
            cout << prompt << "\"special\" coefficient " << i << endl;
            (poly.monoFracs.at(i)).showInfo();
            cout << endl;
        }
    }
    else
    {
        cout << prompt << "coefficient = " << coefficient << endl;
        for (size_t i = 0; i < functions.size(); i++)
        {
            (functions.at(i)).showInfo();
        }
    }
    cout << prompt << "variables size = " << variables.size() << endl;
    for (size_t i = 0; i < variables.size(); i++)
    {
        (variables.at(i)).showInfo();
    }

    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return ;
}

// variable, function, poly (TODO), coefficient
bool monoInfo::operator<(const monoInfo &mono) const
{
    const vector<variableInfo> &vars1 = variables;
    const vector<variableInfo> &vars2 = mono.variables;

    // variable sort
    string vars1Str;
    for(const auto &v : vars1)
    {
        auto tmp = v.printStr();
        vars1Str.append(tmp);
    }
    string vars2Str;
    for(const auto &v : vars2)
    {
        auto tmp = v.printStr();
        vars2Str.append(tmp);
    }
    return (vars1Str < vars2Str);
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
    vector<funcInfo> &functions = monoFinal.functions;
    vector<variableInfo> &variables = monoFinal.variables;
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

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2, const char &op)
{
    monoInfo monoFinal;
    auto &poly = monoFinal.poly;
    auto &monoFracs = poly.monoFracs;
    if(op != '+')
    {
        cout << "ERROR: mergeMonomial: op is wrong! " << op << endl;
    }
    monoFracInfo monoFrac1;
    monoFracInfo monoFrac2;
    monoFrac1.numerator = mono1;
    monoFrac2.numerator = mono2;
    monoFracs.push_back(monoFrac1);
    monoFracs.push_back(monoFrac2);

    return monoFinal;
}

monoInfo extractInfoKernel(const ast_ptr &expr)
{
    // fprintf(stderr, "extractInfoKernel: at the begin: expr = %s\n", PrintExpression(expr).c_str());
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    vector<funcInfo> &functions = monoFinal.functions;
    vector<variableInfo> &variables = monoFinal.variables;
    funcInfo funcTmp;
    variableInfo variableTmp;

    string exprType = expr->type();
    vector<ast_ptr> items;
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
            auto monoFracsTmp = extractFracInfo(exprsTmp);
            polyInfo poly;
            for(auto monoFracTmp : monoFracsTmp)
            {
                (poly.monoFracs).push_back(monoFracTmp);
            }
            std::sort((poly.monoFracs).begin(), (poly.monoFracs).end());
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
        ast_ptr &lhs = binOpPtr->getLHS();
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        ast_ptr &rhs = binOpPtr->getRHS();
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else if (op == '+')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '+'\n");
        ast_ptr &lhs = binOpPtr->getLHS();
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        ast_ptr &rhs = binOpPtr->getRHS();
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        monoFinal = mergeMonomial(monoTmp1, monoTmp2, '+');
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '%c'\n", op);
    }
    return monoFinal;
}

vector<monoInfo> extractInfo(const vector<ast_ptr> &exprs)
{
    // fprintf(stderr, "extractInfo: start--------\n");
    vector<monoInfo> results;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        ast_ptr exprTmp = exprs.at(i)->Clone();
        monoInfo monoTmp = extractInfoKernel(exprTmp);
        if(monoTmp.coefficient != 0)
        {
            vector<variableInfo> &variables = monoTmp.variables;
            std::sort(variables.begin(), variables.end());
            results.push_back(monoTmp);
        }
    }

    // print information of info
    // for (long unsigned int i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "extractInfo: The monoInfo No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    // fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

vector<monoInfo> mergePolynomial(const vector<monoInfo> &info)
{
    // fprintf(stderr, "mergePolynomial: start--------\n");
    size_t size = info.size();
    bool *merged = new bool[size]{};
    size_t i = 0, j = 0;
    vector<monoInfo> results;
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
    // TODO: enhance sort. Now just sort variables.
    std::sort(results.begin(), results.end());

    // print information of info
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "mergePolynomial: The Monomial No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    delete []merged;
    // fprintf(stderr, "mergePolynomial: end----------\n");
    return results;
}
