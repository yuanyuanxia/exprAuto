#include "monoFracInfo.hpp"
#include "preprocess.hpp"

#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

void monoFracInfo::showInfo()
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "monoFracInfo::showInfo: ";
    cout << prompt << "start-----------" << endl;
    monoInfo &numerator = this->numerator;
    monoInfo &denominator = this->denominator;

    cout << prompt << "numerator: " << endl;
    numerator.showInfo();
    cout << prompt << "denominator: " << endl;
    denominator.showInfo();

    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return ;
}


monoFracInfo extractFracInfoKernel(const ast_ptr &expr)
{
    // fprintf(stderr, "extractFracInfoKernel: at the begin: expr = %s\n", PrintExpression(expr).c_str());
    monoFracInfo monoFracFinal;

    monoInfo &numerator = monoFracFinal.numerator;
    monoInfo &denominator = monoFracFinal.denominator;

    double &coefficient = numerator.coefficient;
    vector<funcInfo> &functions = numerator.functions;
    vector<variableInfo> &variables = numerator.variables;
    funcInfo funcTmp;
    variableInfo variableTmp;

    string exprType = expr->type();
    vector<ast_ptr> items;
    if (exprType == "Number")
    {
        // fprintf(stderr, "extractFracInfoKernel: number\n");
        NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
        coefficient = numberPtr->getNumber();
        return monoFracFinal;
    }
    else if (exprType == "Variable")
    {
        // fprintf(stderr, "extractFracInfoKernel: variable\n");
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        variableTmp.name = variablePtr->getVariable();
        variableTmp.degree = 1;
        variables.push_back(variableTmp);
        return monoFracFinal;
    }
    else if (exprType == "Call")
    {
        // fprintf(stderr, "extractFracInfoKernel: variable\n");
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        funcTmp.callee = callPtr->getCallee();
        for(size_t i = 0; i < (callPtr->getArgs()).size(); ++i)
        {
            auto argAST = (callPtr->getArgs().at(i))->Clone();
            auto exprsTmp = extractItems(argAST);
            auto monosTmp = extractFracInfo(exprsTmp);
            polyInfo poly;
            for(auto monoTmp : monosTmp)
            {
                (poly.monoFracs).push_back(monoTmp);
                // (poly.monos).push_back(monoTmp.numerator);
            }
            std::sort((poly.monoFracs).begin(), (poly.monoFracs).end());
            // std::sort((poly.monos).begin(), (poly.monos).end());
            (funcTmp.args).push_back(poly);
        }
        functions.push_back(funcTmp);
        return monoFracFinal;
    }
    if (exprType != "Binary")
    {   // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractFracInfoKernel: expr is neither number, variable, call or binary\n");
        return monoFracFinal;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    ast_ptr &lhs = binOpPtr->getLHS();
    ast_ptr &rhs = binOpPtr->getRHS();
    if (op == '*')
    {
        // fprintf(stderr, "extractFracInfoKernel: expr op is '*'\n");
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        numerator = mergeMonomial(monoTmp1, monoTmp2);
    }
    else if (op == '/')
    {
        numerator = extractInfoKernel(lhs);
        denominator = extractInfoKernel(rhs);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractFracInfoKernel: expr contains '%c'\n", op);
        exit(EXIT_FAILURE);
    }
    return monoFracFinal;
}

vector<monoFracInfo> extractFracInfo(const vector<ast_ptr> &exprs)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "extractFracInfo: ";
    cout << prompt << "start-----------" << endl;

    vector<monoFracInfo> results;
    for (size_t i = 0; i < exprs.size(); i++)
    {
        const ast_ptr &exprTmp = exprs.at(i);
        monoFracInfo monoFracTmp = extractFracInfoKernel(exprTmp);
        if(monoFracTmp.coefficient != 0)
        {
            vector<variableInfo> &variables = monoFracTmp.numerator.variables;
            std::sort(variables.begin(), variables.end());
            vector<variableInfo> &variables1 = monoFracTmp.denominator.variables;
            std::sort(variables1.begin(), variables1.end());
            results.push_back(monoFracTmp);
        }
    }

    // print information of info
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     cout << prompt << "The monoFracInfo No." << i << endl;
    //     (results.at(i)).showInfo();
    //     cout << endl;
    // }
    cout << prompt << "end-----------" << endl;
    callCount--;
    callLevel--;
    return results;
}

bool checkMonoFrac(const vector<monoFracInfo> &infos)
{
    // fprintf(stderr, "checkMonoFrac: start--------\n");
    bool flag = true;
    for (size_t i = 0; i < infos.size(); i++)
    {
        const auto &info = infos.at(i);
        const auto &denominator = info.denominator;
        const auto &vars = denominator.variables;
        const auto &funcs = denominator.functions;
        const auto &poly = denominator.poly;
        const auto &coefficient = denominator.coefficient;
        // if denominator is not 1, just set the flag to false and then break out the loop.
        if (coefficient != 1.0 || vars.size() != 0 || funcs.size() != 0 || poly.monos.size() != 0)
        {
            flag = false;
            break;
        }
    }
    // fprintf(stderr, "checkMonoFrac: end----------\n");
    return flag;
}

bool monoFracInfo::operator<(const monoFracInfo &monoFrac) const
{
    const auto &numerator1 = numerator;
    const auto &denominator1 = denominator;

    const auto &numerator2 = monoFrac.numerator;
    const auto &denominator2 = monoFrac.denominator;

    if(denominator1 < denominator2) {
        return true;
    }
    else if(denominator1 == denominator2 && numerator1 < numerator2)
    {
        return true;
    }
    return false;
}
