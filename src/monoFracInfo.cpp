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
                (poly.monos).push_back(monoTmp.numerator);
            }
            // std::sort((poly.monoFracs).begin(), (poly.monoFracs).end()); // TODO: implement it
            std::sort((poly.monos).begin(), (poly.monos).end());
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

vector<monoFracInfo> mergePolynomialFrac(const vector<monoFracInfo> &info)
{
    // // fprintf(stderr, "mergePolynomialFrac: start--------\n");
    // size_t size = info.size();
    // bool *merged = new bool[size]{};
    // size_t i = 0, j = 0;
    // vector<monoFracInfo> results;
    // while (i < info.size())
    // {
    //     if(merged[i])
    //     {
    //         i++;
    //         continue;
    //     }
    //     monoFracInfo tmp = info.at(i);
    //     merged[i] = true;
    //     j = i + 1;
    //     while (j < info.size())
    //     {
    //         monoFracInfo tmp1 = info.at(j);
    //         // if(tmp.hasCommonTerm(tmp1))
    //         {
    //             tmp.combine(tmp1);
    //             merged[j] = true;
    //         }
    //         j++;
    //     }
    //     results.push_back(tmp);
    // }
    // // TODO: check sort. Eg: b*d + a*b + b*b + a*a, output should be a*a + a*b + b*b + b*d, but is a*a + b*b + a*b + b*d;
    // std::sort(results.begin(), results.end());

    // // print information of info
    // // for (size_t i = 0; i < results.size(); i++)
    // // {
    // //     fprintf(stderr, "mergePolynomialFrac: The Monomial No.%lu: \n", i);
    // //     (results.at(i)).showInfo();
    // //     fprintf(stderr, "\n");
    // // }
    // delete []merged;
    // // fprintf(stderr, "mergePolynomialFrac: end----------\n");
    // return results;
}