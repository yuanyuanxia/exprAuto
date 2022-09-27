#include <algorithm>
#include "geneExpr.hpp"

std::unique_ptr<ExprAST> geneFunctionAST(const funcInfo &func)
{
    std::string callee = func.callee;
    std::vector<std::unique_ptr<ExprAST>> argASTs;
    for(const polyInfo &arg : func.args)
    {
        std::unique_ptr<ExprAST> argAST = geneExprAST(arg.monos);
        argASTs.push_back(std::move(argAST));
    }
    return std::make_unique<CallExprAST>(callee, std::move(argASTs));
}

std::unique_ptr<ExprAST> geneMonomialAST(const monoInfo &monomial)
{
    const std::vector<variableInfo> &vars = monomial.variables;
    const std::vector<funcInfo> &funcs = monomial.functions;
    const polyInfo &poly = monomial.poly;
    if (vars.size() == 0 && funcs.size() == 0 && poly.monos.size() == 0) // number
    {
        return std::make_unique<NumberExprAST>(monomial.coefficient);
    }
    else if ((vars.size() == 1) && (vars.at(0).degree == 1) && (monomial.coefficient == 1) && (funcs.size() == 0) && poly.monos.size() == 0) // single variable
    {
        return std::make_unique<VariableExprAST>(vars.at(0).name);
    }
    else if (vars.size() == 0 && monomial.coefficient == 1 && funcs.size() == 1 && poly.monos.size() == 0) // single function
    {
        funcInfo func = funcs.at(0);
        return geneFunctionAST(func);
    }
    else // many variables or functions
    {
        // set monomial's coefficient
        std::unique_ptr<ExprAST> newExpr = nullptr;
        if(monomial.coefficient != 1)
        {
            newExpr = std::make_unique<NumberExprAST>(monomial.coefficient);
        }

        // set monomial's functions
        const std::vector<funcInfo> &functions = monomial.functions;
        for (const auto &func : functions)
        {
            std::unique_ptr<ExprAST> funcAST = geneFunctionAST(func);
            newExpr = mulExpr(std::move(newExpr), std::move(funcAST));
        }

        // set the addtional functions in the monomial.
        // NOTE: If there is poly, there is no func, and if there is func, there is no poly
        std::unique_ptr<ExprAST> tempMonos = nullptr;
        for (const auto &mono : poly.monos)
        {
            std::unique_ptr<ExprAST> tempMono = geneMonomialAST(mono);
            tempMonos = addExpr(std::move(tempMonos), std::move(tempMono));
        }
        if (tempMonos != nullptr)
        {
            newExpr = mulExpr(std::move(newExpr), std::move(tempMonos));
        }

        // set monomial's variables
        for (const auto &var : vars)
        {
            std::unique_ptr<ExprAST> elementAST = std::make_unique<VariableExprAST>(var.name);
            std::unique_ptr<ExprAST> varAST = nullptr;
            for (int j = 0; j < var.degree; j++)
            {
                varAST = mulExpr(std::move(varAST), std::move(elementAST->Clone()));
            }
            newExpr = mulExpr(std::move(newExpr), std::move(varAST));
        }

        return newExpr;
    }
}

std::unique_ptr<ExprAST> geneExprAST(const std::vector<monoInfo> &monos)
{
    // fprintf(stderr, "geneExprAST: start--------\n");
    if (monos.size() == 0)
    {
        fprintf(stderr, "ERROR: geneExprAST: input is NONE!\n");
    }
    
    std::unique_ptr<ExprAST> newExpr, tempAST;
    for (const auto &mono: monos)
    {
        tempAST = geneMonomialAST(mono);
        newExpr = addExpr(std::move(newExpr), std::move(tempAST));
    }
    // fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr;
}

std::unique_ptr<ExprAST> geneExprASTYHR(std::vector<monoInfo> &info)
{
    fprintf(stderr, "geneExprAST: start--------\n");
    // info为monomial对象容器
    //表达式为数字单项式
    if (info.size() == 1 && info.at(0).variables.size() == 0)
    {
        std::unique_ptr<NumberExprAST> newExpr = std::make_unique<NumberExprAST>(NumberExprAST(info.at(0).coefficient));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }
    //表达式为变量单项式
    if (info.size() == 1 && info.at(0).variables.size() == 1 && info.at(0).variables.at(0).degree == 1 && info.at(0).coefficient == 1)
    {
        std::unique_ptr<VariableExprAST> newExpr = std::make_unique<VariableExprAST>(VariableExprAST(info.at(0).variables.at(0).name));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }

    std::unique_ptr<BinaryExprAST> newExpr = std::make_unique<BinaryExprAST>(BinaryExprAST('+', nullptr, nullptr));
    std::unique_ptr<BinaryExprAST> tempBinaryAST = nullptr;
    std::unique_ptr<BinaryExprAST> binaryASTLhs = nullptr;
    std::unique_ptr<ExprAST> ExprASTASTVar = nullptr;
    std::unique_ptr<ExprAST> tempBinaryASTLhs = nullptr;
    //先创建一�?临时的binaryAST,将�??一�?表达式作为左子树
    for (size_t i = 0; i < info.size(); i++)
    {
        if (i == 0)
        {
            //表达式为数字单项式
            if (info.at(i).variables.size() == 0)
            {
                tempBinaryASTLhs = std::move(std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient)));
            }
            //表达式为变量单项式
            else if ((info.at(i).variables.size() == 1) && (info.at(i).coefficient == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryASTLhs = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                std::vector<std::string> varVec1;

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
                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                    if (info.size() == 1)
                    {
                        fprintf(stderr, "geneExprAST: end--------\n");
                        return binaryASTLhs->Clone();
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(0)));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
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
                ExprASTASTVar = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                std::vector<std::string> varVec2;
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

                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(0)));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
            }
        }

        if (i == 0)
        {
            std::unique_ptr<ExprAST> tempBinaryAST1 = tempBinaryASTLhs.get()->Clone();
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
            std::unique_ptr<ExprAST> tempBinaryAST2 = nullptr;
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
            std::unique_ptr<ExprAST> newExpr1 = newExpr.get()->Clone();
            newExpr = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('+', std::move(newExpr1), nullptr)));
        }
    }
    fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr->Clone();
}
