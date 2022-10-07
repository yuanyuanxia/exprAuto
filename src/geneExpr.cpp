#include <algorithm>
#include "geneExpr.hpp"

using std::string;
using std::vector;

ast_ptr geneFunctionAST(const funcInfo &func)
{
    string callee = func.callee;
    vector<ast_ptr> argASTs;
    for(const polyInfo &arg : func.args)
    {
        ast_ptr argAST = geneExprAST(arg.monoFracs);
        argASTs.push_back(std::move(argAST));
    }
    return makePtr<CallExprAST>(callee, std::move(argASTs));
}

ast_ptr geneMonomialAST(const monoInfo &monomial)
{
    const vector<variableInfo> &vars = monomial.variables;
    const vector<funcInfo> &funcs = monomial.functions;
    const polyInfo &poly = monomial.poly;
    if (vars.size() == 0 && funcs.size() == 0 && poly.monos.size() == 0) // number
    {
        return makePtr<NumberExprAST>(monomial.coefficient);
    }
    else if ((vars.size() == 1) && (vars.at(0).degree == 1) && (monomial.coefficient == 1) && (funcs.size() == 0) && poly.monos.size() == 0) // single variable
    {
        return makePtr<VariableExprAST>(vars.at(0).name);
    }
    else if (vars.size() == 0 && monomial.coefficient == 1 && funcs.size() == 1 && poly.monos.size() == 0) // single function
    {
        funcInfo func = funcs.at(0);
        return geneFunctionAST(func);
    }
    else // many variables or functions
    {
        // set monomial's coefficient
        ast_ptr newExpr = nullptr;
        if(monomial.coefficient != 1)
        {
            newExpr = makePtr<NumberExprAST>(monomial.coefficient);
        }

        // set monomial's functions
        const vector<funcInfo> &functions = monomial.functions;
        for (const auto &func : functions)
        {
            ast_ptr funcAST = geneFunctionAST(func);
            newExpr = mulExpr(std::move(newExpr), std::move(funcAST));
        }

        // set the addtional functions in the monomial.
        // NOTE: If there is poly, there is no func, and if there is func, there is no poly
        ast_ptr tempMonos = nullptr;
        for (const auto &mono : poly.monos)
        {
            ast_ptr tempMono = geneMonomialAST(mono);
            tempMonos = addExpr(std::move(tempMonos), std::move(tempMono));
        }
        if (tempMonos != nullptr)
        {
            newExpr = mulExpr(std::move(newExpr), std::move(tempMonos));
        }

        // set monomial's variables
        for (const auto &var : vars)
        {
            ast_ptr elementAST = makePtr<VariableExprAST>(var.name);
            ast_ptr varAST = nullptr;
            for (int j = 0; j < var.degree; j++)
            {
                varAST = mulExpr(std::move(varAST), std::move(elementAST->Clone()));
            }
            newExpr = mulExpr(std::move(newExpr), std::move(varAST));
        }

        return newExpr;
    }
}

ast_ptr geneExprAST(const vector<monoInfo> &monos)
{
    // fprintf(stderr, "geneExprAST: start--------\n");
    if (monos.size() == 0)
    {
        fprintf(stderr, "ERROR: geneExprAST: input is NONE!\n");
    }
    
    ast_ptr newExpr, tempAST;
    for (const auto &mono: monos)
    {
        tempAST = geneMonomialAST(mono);
        newExpr = addExpr(std::move(newExpr), std::move(tempAST));
    }
    // fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr;
}

ast_ptr geneFunctionASTNew(const funcInfo &func)
{
    string callee = func.callee;
    vector<ast_ptr> argASTs;
    for(const polyInfo &arg : func.args)
    {
        ast_ptr argAST = geneExprAST(arg.monoFracs);
        argASTs.push_back(std::move(argAST));
    }
    return makePtr<CallExprAST>(callee, std::move(argASTs));
}

ast_ptr geneMonoFracAST(const monoFracInfo &monoFrac)
{
    const monoInfo &numerator = monoFrac.numerator;
    const monoInfo &denominator = monoFrac.denominator;

    auto numeratorAST = geneMonomialAST(numerator);
    auto denominatorAST = geneMonomialAST(denominator);
    ast_ptr tmpOne = makePtr<NumberExprAST>(1.0);
    if(isEqual(denominatorAST, tmpOne))
    {
        return numeratorAST;
    }
    else
    {
        return divExpr(numeratorAST, denominatorAST);
    }
}

ast_ptr geneExprAST(const vector<monoFracInfo> &monoFracs)
{
    // fprintf(stderr, "geneExprAST: start--------\n");
    if (monoFracs.size() == 0)
    {
        fprintf(stderr, "ERROR: geneExprAST: input is NONE!\n");
    }
    
    ast_ptr newExpr, tempAST;
    for (const auto &monoFrac: monoFracs)
    {
        tempAST = geneMonoFracAST(monoFrac);
        newExpr = addExpr(std::move(newExpr), std::move(tempAST));
    }
    // fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr;
}

ast_ptr geneExprASTYHR(vector<monoInfo> &info)
{
    fprintf(stderr, "geneExprAST: start--------\n");
    // info为monomial对象容器
    //表达式为数字单项式
    if (info.size() == 1 && info.at(0).variables.size() == 0)
    {
        std::unique_ptr<NumberExprAST> newExpr = makePtr<NumberExprAST>(NumberExprAST(info.at(0).coefficient));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }
    //表达式为变量单项式
    if (info.size() == 1 && info.at(0).variables.size() == 1 && info.at(0).variables.at(0).degree == 1 && info.at(0).coefficient == 1)
    {
        std::unique_ptr<VariableExprAST> newExpr = makePtr<VariableExprAST>(VariableExprAST(info.at(0).variables.at(0).name));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }

    std::unique_ptr<BinaryExprAST> newExpr = makePtr<BinaryExprAST>(BinaryExprAST('+', nullptr, nullptr));
    std::unique_ptr<BinaryExprAST> tempBinaryAST = nullptr;
    std::unique_ptr<BinaryExprAST> binaryASTLhs = nullptr;
    ast_ptr ExprASTASTVar = nullptr;
    ast_ptr tempBinaryASTLhs = nullptr;
    //先创建一�?临时的binaryAST,将�??一�?表达式作为左子树
    for (size_t i = 0; i < info.size(); i++)
    {
        if (i == 0)
        {
            //表达式为数字单项式
            if (info.at(i).variables.size() == 0)
            {
                tempBinaryASTLhs = std::move(makePtr<NumberExprAST>(NumberExprAST(info.at(i).coefficient)));
            }
            //表达式为变量单项式
            else if ((info.at(i).variables.size() == 1) && (info.at(i).coefficient == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryASTLhs = std::move(makePtr<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                vector<string> varVec1;

                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec1.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对�?�器排序
                std::sort(varVec1.begin(), varVec1.end());
                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec1.size(); k++)
                // {
                //     std::cout << varVec1.at(k) << std::endl;
                // }
                // std::cout << "print end" << std::endl;

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;
                    std::unique_ptr<NumberExprAST> numAST = makePtr<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    binaryASTLhs = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        ast_ptr varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        ast_ptr tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                    if (info.size() == 1)
                    {
                        fprintf(stderr, "geneExprAST: end--------\n");
                        return binaryASTLhs->Clone();
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec1.at(0)));

                    binaryASTLhs = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        ast_ptr varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        ast_ptr tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                tempBinaryASTLhs = binaryASTLhs->Clone();
            }
        }
        else
        {
            // std::cout << "here" << std::endl;

            //表达式为变量单项式
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && i > 0 && info.at(i).coefficient == 1)
            {
                ExprASTASTVar = std::move(makePtr<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                vector<string> varVec2;
                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec2.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对�?�器排序
                std::sort(varVec2.begin(), varVec2.end());

                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec2.size(); k++)
                // {
                //     std::cout << varVec2.at(k) << std::endl;
                // }

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;

                    std::unique_ptr<NumberExprAST> numAST = makePtr<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    tempBinaryAST = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        ast_ptr varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        ast_ptr tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec2.at(0)));

                    tempBinaryAST = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = makePtr<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        ast_ptr varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        ast_ptr tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(makePtr<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
            }
        }

        if (i == 0)
        {
            ast_ptr tempBinaryAST1 = tempBinaryASTLhs.get()->Clone();
            newExpr->setLHS(tempBinaryAST1);
            if (info.size() == 1)
            {
                fprintf(stderr, "geneExprAST: end--------\n");
                return tempBinaryAST1->Clone();
            }
            continue;
        }
        else
        {
            ast_ptr tempBinaryAST2 = nullptr;
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && (info.at(i).coefficient == 1))
            {
                tempBinaryAST2 = std::move(ExprASTASTVar.get()->Clone());
            }
            else
            {
                tempBinaryAST2 = std::move(tempBinaryAST.get()->Clone());
            }
            newExpr->setRHS(tempBinaryAST2);
        }
        if (i != (info.size() - 1))
        {
            ast_ptr newExpr1 = newExpr.get()->Clone();
            newExpr = std::move(makePtr<BinaryExprAST>(BinaryExprAST('+', std::move(newExpr1), nullptr)));
        }
    }
    fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr->Clone();
}
